// Fill out your copyright notice in the Description page of Project Settings.

#include "ExecutingQuest.h"
#include "CoreConditionProgress.h"
#include "CoreGameInstance.h"
#include "QuestComponent.h"
#include "UE4LogImpl.h"
#include "GameSystemManager.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "CoreCharacterStateBase.h"
#include "Engine/AssetManager.h"
#include "QuestDetail.h"
#include "Engine/StreamableManager.h"
#include "Quest.h"
#include "QuestForestSubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "QuestDetailNodeItem.h"
#include "ConditionTriggerSystem.h"
#include "ScenarioSystem.h"
#include "AsyncPlayScenario.h"
#include "Scenario.h"
#include "GameFrameworkUtils.h"
#include "ConditionBlueprintLibrary.h"
#include "ScenarioComponent.h"
#include "QuestSystem.h"

void UExecutingQuest::Initialize(UQuest* InQuest) {
	Quest = InQuest;
	ID = Quest->ID;

	auto QuestComponent = GetQuestComponent();

	if (Quest.IsValid() && Quest->QuestDetail.Get() && Quest->QuestDetail->Root) {
		SetNodeID(Quest->QuestDetail->Root->ID);
	}
	else {
		UE_LOG(GameCore, Error, TEXT("[UExecutingQuest::Initialize]非法任务"));
	}
}

void UExecutingQuest::Uninitiliaize() {
	for (auto QuestProgress : QuestProgresses) {
		QuestProgress->Uninitialize();
	}

	QuestProgresses.Empty();

	auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
	auto ConditionTriggerSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>();
	ConditionTriggerSystem->UnfollowConditions(ConditionTriggerHandler);
}

const FGuid& UExecutingQuest::GetID() const {
	return ID;
}

UQuestDetailNode* UExecutingQuest::GetCurrentNode() {
	return Quest->QuestDetail->GetNodeByID(NodeID);
}

UQuest* UExecutingQuest::GetQuest() {
	return Quest.Get();
}

const TArray<UCoreConditionProgress*>& UExecutingQuest::GetQuestProgresses() const {
	return QuestProgresses;
}

const TArray<UCoreReward*>& UExecutingQuest::GetQuestRewards() const {
	auto Node = Quest->QuestDetail->GetNodeByID(NodeID);
	UQuestDetailNodeItem* NodeItem = Cast<UQuestDetailNodeItem>(Node);
	return NodeItem->Rewards;
}

bool UExecutingQuest::IsComplete() const {
	return UConditionBlueprintLibrary::DoesProgressesSatisfy(QuestProgresses);
}

UQuestComponent* UExecutingQuest::GetQuestComponent() {
    auto PlayerState = Cast<ACoreCharacterStateBase>(GetOuter());
	if (PlayerState && PlayerState->QuestComponent) {
        return PlayerState->QuestComponent;
	}
	return nullptr;
}

bool UExecutingQuest::IsLastNode() {
	auto Node = Quest->QuestDetail->GetNodeByID(NodeID);
	return Node->NextNodes.Num() == 0;
}

void UExecutingQuest::StepNextNode(int StepIndex) {
	auto Node = Quest->QuestDetail->GetNodeByID(NodeID);
	if (!Node->NextNodes.IsValidIndex(StepIndex)) {
		return;
	}
	//这里取下个节点
	SetNode(Node->NextNodes[StepIndex]);
}

int UExecutingQuest::GetCurrentCommitUnitID() const {
	auto Node = Quest->QuestDetail->GetNodeByID(NodeID);
	UQuestDetailNodeItem* NodeItem = Cast<UQuestDetailNodeItem>(Node);
	if (NodeItem && NodeItem->CommitNPC.UnitID) {
		return NodeItem->CommitNPC.UnitID;
	}
	return 0;
}

void UExecutingQuest::SetNodeID(const FGuid& InNodeID) {
	UE_LOG(GameCore, Log, TEXT("推进任务节点，QID:%s NodeID:%s"), *ID.ToString(), *NodeID.ToString());

	if (NodeID != InNodeID) {
		auto Node = Quest->QuestDetail->GetNodeByID(InNodeID);
		if (Node) {
			SetNode(Node);
		}
	}
}

void UExecutingQuest::CommitQuest(int UnitID) {
	if (IsComplete()) {
		auto Node = Quest->QuestDetail->GetNodeByID(NodeID);
		UQuestDetailNodeItem* NodeItem = Cast<UQuestDetailNodeItem>(Node);
		if (NodeItem && NodeItem->CommitNPC.UnitID == UnitID) {
			UE_LOG(GameCore, Log, TEXT("提交任务，QID:%s NodeID:%s, UnitID:%d"), *ID.ToString(), *NodeID.ToString(), UnitID);
			NotifyPlayScenario();
		}
	}
}

void UExecutingQuest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UExecutingQuest, ID, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UExecutingQuest, NodeID, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UExecutingQuest, QuestProgresses, Params);
}

bool UExecutingQuest::IsSupportedForNetworking() const {
    return true;
}

void UExecutingQuest::OnAllProgressesSatisfy(FConditionTriggerHandler Handler) {
	//所有任务进度都变成完成
	auto NetMode = GetWorld()->GetNetMode();
	if (NetMode == ENetMode::NM_Standalone
	|| NetMode == ENetMode::NM_DedicatedServer) {
		auto Node = Quest->QuestDetail->GetNodeByID(NodeID);
		UQuestDetailNodeItem* NodeItem = Cast<UQuestDetailNodeItem>(Node);
		if (!NodeItem || (NodeItem && (NodeItem->CommitNPC.UnitID == 0 || NodeItem->bAutoPlayScenario))) {
			NotifyPlayScenario();
		}
	}
}

void UExecutingQuest::OnRep_QuestID() {
	auto QuestForestSubsystem = Cast<UQuestForestSubsystem>(USubsystemBlueprintLibrary::GetGameInstanceSubsystem(this, UQuestForestSubsystem::StaticClass()));
	Quest = QuestForestSubsystem->GetQuestByID(ID);
	if (!Quest.IsValid()) {
		UE_LOG(GameCore, Error, TEXT("客户端任务表读表错误%s"), *ID.ToString());
	}
	else {
		//同步过来，要通知
		GetQuestComponent()->OnQuestChanged();
	}
}

void UExecutingQuest::OnRep_NodeID() {
	UE_LOG(GameCore, Log, TEXT("收到任务节点推进通知，QID:%s NodeID:%s"), *ID.ToString(), *NodeID.ToString());
}

void UExecutingQuest::OnRep_Progress(const TArray<UCoreConditionProgress*>& OldProgresses) {

}

void UExecutingQuest::OnRep_Rewards() {
	
}

void UExecutingQuest::NotifyPlayScenario() {
	UE_LOG(GameCore, Log, TEXT("任务节点进度完成，QID:%s NodeID:%s"), *ID.ToString(), *NodeID.ToString());
	auto Node = Quest->QuestDetail->GetNodeByID(NodeID);
	UQuestDetailNodeItem* NodeItem = Cast<UQuestDetailNodeItem>(Node);
	if (NodeItem) {
		if (ScenarioLoadedHandle.IsValid()) {
			ScenarioLoadedHandle->CancelHandle();
			ScenarioLoadedHandle.Reset();
		}

		if (NodeItem->Scenario.GetLongPackageName().IsEmpty()) {
			PlayScenarioCompleted(nullptr, 0);
		}
		else {
			auto LoadScenarioPath = NodeItem->Scenario.ToSoftObjectPath();
			UE_LOG(GameCore, Log, TEXT("任务节点开始加载剧情，QID:%s NodeID:%s Scenario:%s"), *ID.ToString(), *NodeID.ToString(), *LoadScenarioPath.ToString());
			ScenarioLoadedHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(LoadScenarioPath, FStreamableDelegate::CreateUObject(this, &UExecutingQuest::NotifyPlayScenarioAfterLoaded, LoadScenarioPath));
		}
	}
	else {
		UE_LOG(GameCore, Error, TEXT("不支持的任务节点类型"));
	}
}

void UExecutingQuest::NotifyPlayScenarioAfterLoaded(FSoftObjectPath LoadScenarioPath) {
	auto Scenario = Cast<UScenario>(ScenarioLoadedHandle->GetLoadedAsset());
	if (Scenario) {
		UE_LOG(GameCore, Log, TEXT("任务节点开始播放剧情，QID:%s NodeID:%s Scenario:%s"), *ID.ToString(), *NodeID.ToString(), *LoadScenarioPath.ToString());
		auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
		auto PlayerState = Cast<ACoreCharacterStateBase>(GetOuter());
		auto AsyncPlayScenario = GameInstance->GameSystemManager->GetSystemByClass<UScenarioSystem>()->PlayScenario(PlayerState->ScenarioComponent, Scenario);
		AsyncPlayScenario->OnComplete.AddDynamic(this, &UExecutingQuest::PlayScenarioCompleted);
	}
	else {
		UE_LOG(GameCore, Error, TEXT("任务节点剧情加载失败，QID:%s NodeID:%s Scenario:%s"), *ID.ToString(), *NodeID.ToString(), *LoadScenarioPath.ToString());
	}
}

void UExecutingQuest::PlayScenarioCompleted(UScenario* PlayScenario, int ReturnIndex) {
	UE_LOG(GameCore, Log, TEXT("任务节点播放剧情完成，推动剧情，QID:%s NodeID:%s"), *ID.ToString(), *NodeID.ToString());

	auto PlayerState = Cast<ACoreCharacterStateBase>(GetOuter());
	auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
	GameInstance->GameSystemManager->GetSystemByClass<UQuestSystem>()->PushQuest(PlayerState->QuestComponent, ID, ReturnIndex);
}

void UExecutingQuest::SetNode(UQuestDetailNode* InNode) {
	NodeID = InNode->ID;
	if (GetWorld()->GetNetMode() == ENetMode::NM_Standalone) {
		OnRep_NodeID();
	}

	for (auto QuestProgress : QuestProgresses) {
		QuestProgress->Uninitialize();
	}

	QuestProgresses.Empty();

	auto Owner = Cast<AActor>(GetOuter());

	UQuestDetailNodeItem* NodeItem = Cast<UQuestDetailNodeItem>(InNode);
	if (NodeItem) {
		for (auto Condition : NodeItem->ConditionList) {
			auto ConditionProgress = Condition->GenerateConditionProgress(Owner);
			ConditionProgress->Initialize();
			QuestProgresses.Add(ConditionProgress);
		}
	}
	else {
		UE_LOG(GameCore, Error, TEXT("不支持的任务节点类型"));
	}

	auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
	auto ConditionTriggerSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>();

	ConditionTriggerSystem->UnfollowConditions(ConditionTriggerHandler);

	FOnAllProgressesSatisfyDelegate Callback;
	Callback.BindUFunction(this, TEXT("OnAllProgressesSatisfy"));

	ConditionTriggerSystem->FollowConditions(ConditionTriggerHandler, QuestProgresses, Callback);
}