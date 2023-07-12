// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "CoreGameInstance.h"
#include "QuestSystem.h"
#include "AcceptableQuest.h"
#include "ExecutingQuest.h"
#include "GameSystemManager.h"
#include "EventSystem.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "GameEntity.h"
#include "CoreConditionProgress.h"
#include "GameFramework/PlayerState.h"
#include "QuestForestSubsystem.h"
#include "QuestTree.h"
#include "Quest.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "UE4LogImpl.h"
#include "QuestSetting.h"
#include "QuestNPCAcquirePredicate.h"
#include "AcquireNPCsCondition.h"
#include "NPCSystem.h"
#include "SortUtils.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

    auto NetMode = GetWorld()->GetNetMode();
    switch (NetMode) {
    case ENetMode::NM_Client:
    {
        //如果是运行客户端，每秒tick一次
        SetComponentTickInterval(1);
    }
    case ENetMode::NM_Standalone:
    {
        SetComponentTickEnabled(true);
        //todo，没有读盘机制，先上来就加载任务
        RefreshAcceptableQuests();
    }
    break;
    default:
    {
        SetComponentTickEnabled(false);
    }
    break;
    }
}


void UQuestComponent::AcceptQuest(const FGuid& ID) {
    auto PlayerState = Cast<APlayerState>(GetOwner());
    check(PlayerState);

    auto GameEntity = Cast<IGameEntity>(PlayerState->GetPawn());
    if (GameEntity) {
        auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
        auto Request = NewObject<UAcceptQuestRequesEvent>();
        Request->EntityId = GameEntity->GetEntityID();
        Request->ID = ID;

        GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEventToServer(Request, true);
    }
}

void UQuestComponent::PushQuest(const FGuid& ID, int StepIndex) {
    auto PlayerState = Cast<APlayerState>(GetOwner());
    check(PlayerState);

    auto GameEntity = Cast<IGameEntity>(PlayerState->GetPawn());
    if (GameEntity) {
        auto Request = NewObject<UPushQuestRequesEvent>();
        Request->EntityId = GameEntity->GetEntityID();
        Request->ID = ID;
        Request->StepIndex = StepIndex;

        auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
        GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEventToServer(Request, false);
    }
}

void UQuestComponent::OnQuestChanged() {
    TArray<FSoftObjectPath> TargetsToStream;
    for (int Index = 0; Index < ExecutingQuests.Num(); ++Index) {
        if (ExecutingQuests[Index]->GetQuest() == nullptr) {
            //quest还没同步过来，等待同步
            return;
        }
        TargetsToStream.Add(ExecutingQuests[Index]->GetQuest()->QuestDetail.ToSoftObjectPath());
    }

    if (QuestLoadedHandle.IsValid()) {
        QuestLoadedHandle->CancelHandle();
        QuestLoadedHandle.Reset();
    }

    //这里要先准备资源
    if (TargetsToStream.Num() > 0) {
        QuestLoadedHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(TargetsToStream, FStreamableDelegate::CreateUObject(this, &UQuestComponent::NotifyQuestChangedAfterLoaded));
    }
    else {
        NotifyQuestChangedAfterLoaded();
    }
}

void UQuestComponent::NotifyQuestChangedAfterLoaded() {
    UE_LOG(GameCore, Log, TEXT("UQuestComponent::NotifyQuestChangedAfterLoaded"));
    QuestLoadedHandle.Reset();

    auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
    auto OnQuestRefreshEvent = NewObject<UOnQuestRefreshEvent>();
    OnQuestRefreshEvent->QuestComponent = this;
    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(OnQuestRefreshEvent);

    RefreshAcceptableQuests();
}

void UQuestComponent::OnFinishQuestChanged() {
    auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
    auto OnFinishQuestsRefreshEvent = NewObject<UOnFinishQuestsRefreshEvent>();
    OnFinishQuestsRefreshEvent->QuestComponent = this;
    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(OnFinishQuestsRefreshEvent);

    RefreshAcceptableQuests();
}

void UQuestComponent::RefreshAcceptableQuests() {
    bDirtyQuests = true;
}

void UQuestComponent::NotifyAcquireNPCs() {
    bAcquireNPCs = true;
    NotifyServerNextTickHandle();
}

void UQuestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    FDoRepLifetimeParams Params;
    Params.Condition = COND_OwnerOnly;

    DOREPLIFETIME_WITH_PARAMS_FAST(UQuestComponent, ExecutingQuests, Params);
    DOREPLIFETIME_WITH_PARAMS_FAST(UQuestComponent, FinishQuests, Params);
}

bool UQuestComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) {
    bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

    for (auto ExecutingQuest : ExecutingQuests) {
        if (IsValid(ExecutingQuest)) {
            WroteSomething |= Channel->ReplicateSubobject(const_cast<UExecutingQuest*>(ExecutingQuest), *Bunch, *RepFlags);

            for (auto QuestProgress : ExecutingQuest->GetQuestProgresses()) {
                if (IsValid(QuestProgress)) {
                    TArray<UCoreConditionProgress*> ProgressesWithChildren;
                    QuestProgress->GetProgressesWithChildren(ProgressesWithChildren);
                    for (auto Progress : ProgressesWithChildren) {
                        if (IsValid(Progress)) {
                            WroteSomething |= Channel->ReplicateSubobject(Progress, *Bunch, *RepFlags);
                        }
                    }
                }
            }
        }
    }

    return WroteSomething;
}

void UQuestComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    RefreshAcceptQuests();
    RefreshNPCAcquire();

    StopServerTick();
}

void UQuestComponent::RefreshAcceptQuests() {
    if (bDirtyQuests) {
        auto PlayerState = Cast<APlayerState>(GetOwner());
        if (PlayerState->GetPawn() == nullptr) {
            return;
        }

        auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
        for (auto Iter = AcceptQuests.CreateConstIterator(); Iter; ++Iter) {
            Iter->Value->StopListen();
            Iter->Value->Uninitialize();
        }
        AcceptQuests.Empty();

        TSet<FGuid> FinishSet;
        for (int Index = 0; Index < FinishQuests.Num(); ++Index) {
            FinishSet.Add(FinishQuests[Index]);
        }
        TSet<FGuid> ExecutingSet;
        for (int Index = 0; Index < ExecutingQuests.Num(); ++Index) {
            ExecutingSet.Add(ExecutingQuests[Index]->GetID());
        }

        //遍历任务，找到新的可接受任务
        TSet<FGuid> VisitIDSet;
        TQueue<UQuest*> VisitQuests;
        UQuestForestSubsystem* QuestForestSubsystem = GameInstance->GetSubsystem<UQuestForestSubsystem>();
        const auto& AllQuestTreeMap = QuestForestSubsystem->GetAllQuestTreeMap();
        for (auto Iter = AllQuestTreeMap.CreateConstIterator(); Iter; ++Iter) {
            auto QuestTree = Iter->Value;
            if (QuestTree->Root) {
                VisitQuests.Enqueue(QuestTree->Root);
                VisitIDSet.Add(QuestTree->Root->ID);
                while (!VisitQuests.IsEmpty()) {
                    UQuest* VisitQuest;
                    VisitQuests.Dequeue(VisitQuest);
                    if (VisitQuest) {
                        if (ExecutingSet.Contains(VisitQuest->ID))
                            continue;
                        if (FinishSet.Contains(VisitQuest->ID)) {
                            //该任务已经完成，遍历下个
                            for (int FollowIndex = 0; FollowIndex < VisitQuest->FollowQuests.Num(); ++FollowIndex) {
                                auto FollowQuest = VisitQuest->FollowQuests[FollowIndex];
                                if (!VisitIDSet.Contains(FollowQuest->ID)) {
                                    VisitIDSet.Add(FollowQuest->ID);
                                    VisitQuests.Enqueue(FollowQuest);
                                }
                            }
                        }
                        else {
                            //检查全部前置
                            bool IsAllPreFinish = true;
                            for (auto PreQuest : VisitQuest->PreQuests) {
                                if (!FinishSet.Contains(PreQuest->ID)) {
                                    IsAllPreFinish = false;
                                    break;
                                }
                            }
                            if (IsAllPreFinish) {
                                //该任务未完成
                                UAcceptableQuest* AcceptableQuest = NewObject<UAcceptableQuest>(this);
                                AcceptableQuest->Initialize(VisitQuest, this);
                                AcceptableQuest->StartListen();
                                AcceptQuests.Add(VisitQuest->ID, AcceptableQuest);
                            }
                        }
                    }
                }
            }
        }

        bDirtyQuests = false;

        auto OnAcceptQuestIdsRefreshEvent = NewObject<UOnAcceptQuestIdsRefreshEvent>();
        OnAcceptQuestIdsRefreshEvent->QuestComponent = this;
        GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(OnAcceptQuestIdsRefreshEvent);
    }
}

void UQuestComponent::RefreshNPCAcquire() {
    if (bAcquireNPCs) {
        //得到需要占用npc的任务
        auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
        auto NPCSystem = GameInstance->GameSystemManager->GetSystemByClass<UNPCSystem>();

        TArray<UExecutingQuest*> AcquireNPCQuests;
        for (int QuestIndex = 0; QuestIndex < ExecutingQuests.Num(); ++QuestIndex) {
            auto ExecutingQuest = ExecutingQuests[QuestIndex];
            const auto& QuestProgresses = ExecutingQuest->GetQuestProgresses();
            for (int ProgressIndex = 0; ProgressIndex < QuestProgresses.Num(); ++ProgressIndex) {
                auto Progress = QuestProgresses[ProgressIndex];
                auto QuestProgress = Cast<UAcquireNPCsConditionProgress>(Progress);
                bool IsValid;
                if (QuestProgress && !QuestProgress->IsComplete(IsValid) && IsValid) {
                    auto AcquireNPCsCondition = Cast<UAcquireNPCsCondition>(QuestProgress->Condition);
                    if (AcquireNPCsCondition && NPCSystem->IsNPCReleaseByContainer(AcquireNPCsCondition->UnitIDContainers)) {
                        AcquireNPCQuests.Add(ExecutingQuest);
                        break;
                    }
                }
            }
        }
        if (AcquireNPCQuests.Num() > 0) {
            //有满足条件的可以占有的NPC，进行优先级查询
            const UQuestSetting* QuestSetting = GetDefault<UQuestSetting>();
            FString NPCAcquirePredicateClassPath = QuestSetting->NPCAcquirePredicateClass.ToString();
            TSubclassOf<UQuestNPCAcquirePredicate> QuestNPCAcquirePredicateClass = UQuestNPCAcquirePredicate::StaticClass();
            if (!NPCAcquirePredicateClassPath.IsEmpty()) {
                TSubclassOf<UQuestNPCAcquirePredicate> LoadClass = StaticLoadClass(UQuestNPCAcquirePredicate::StaticClass(), NULL, *NPCAcquirePredicateClassPath);
                if (LoadClass) {
                    QuestNPCAcquirePredicateClass = LoadClass;
                }
            }
            
            auto QuestNPCAcquirePredicate = Cast<UQuestNPCAcquirePredicate>(QuestNPCAcquirePredicateClass->GetDefaultObject());
            TFunction<bool(UExecutingQuest*, UExecutingQuest*)> CompareFunc = [this, QuestNPCAcquirePredicate](UExecutingQuest* A, UExecutingQuest* B) {
                return QuestNPCAcquirePredicate->Compare(this, A, B);
            };
            USortUtils::SortArray(AcquireNPCQuests, CompareFunc);
            //按优先级从高到低占有NPC
            for (int Index = AcquireNPCQuests.Num() - 1; Index >= 0; --Index) {
                auto AcquireNPCQuest = AcquireNPCQuests[Index];
                const auto& QuestProgresses = AcquireNPCQuest->GetQuestProgresses();
                for (int ProgressIndex = 0; ProgressIndex < QuestProgresses.Num(); ++ProgressIndex) {
                    auto Progress = QuestProgresses[ProgressIndex];
                    auto AcquireNPCsConditionProgress = Cast<UAcquireNPCsConditionProgress>(Progress);
                    bool IsValid;
                    if (AcquireNPCsConditionProgress && !AcquireNPCsConditionProgress->IsComplete(IsValid) && IsValid) {
                        auto AcquireNPCsCondition = Cast<UAcquireNPCsCondition>(AcquireNPCsConditionProgress->Condition);
                        NPCSystem->TryAcquireNPCByContainer(GetOwner(), AcquireNPCsCondition->UnitIDContainers, AcquireNPCsConditionProgress);
                    }
                }
            }
        }

        bAcquireNPCs = false;
    }
}

void UQuestComponent::NotifyServerNextTickHandle() {
    auto NetMode = GetWorld()->GetNetMode();
    if (NetMode == ENetMode::NM_DedicatedServer
        || NetMode == ENetMode::NM_ListenServer) {
        SetComponentTickEnabled(true);
    }
}

void UQuestComponent::StopServerTick() {
    auto NetMode = GetWorld()->GetNetMode();
    if (NetMode == ENetMode::NM_DedicatedServer
        || NetMode == ENetMode::NM_ListenServer) {
        SetComponentTickEnabled(false);
    }
}