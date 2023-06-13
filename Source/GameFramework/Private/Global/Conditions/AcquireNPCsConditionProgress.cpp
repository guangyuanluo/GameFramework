// Fill out your copyright notice in the ();Description page of Project Settings.

#include "AcquireNPCsConditionProgress.h"
#include "AcquireNPCsCondition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "NPCAcquireEvent.h"
#include "NPCReleaseEvent.h"
#include "CoreCharacterStateBase.h"
#include "GameFrameworkUtils.h"
#include "QuestComponent.h"
#include "NPCSystem.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"

void UAcquireNPCsConditionProgress::PostProgressInitialize_Implementation() {
	Super::PostProgressInitialize_Implementation();

	UAcquireNPCsCondition* AcquireNPCsCondition = Cast<UAcquireNPCsCondition>(Condition);
	auto GameInstance = Cast<UCoreGameInstance>(ProgressOwner->GetWorld()->GetGameInstance());
	if (GameInstance) {
		UNPCSystem* NPCSystem = GameInstance->GameSystemManager->GetSystemByClass<UNPCSystem>();
		if (NPCSystem) {
			if (NPCSystem->IsNPCRelease(AcquireNPCsCondition->UnitIDs)) {
				auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
				if (ConditionPlayerState->QuestComponent) {
					ConditionPlayerState->QuestComponent->NotifyAcquireNPCs();
				}
			}
		}
	}
}

bool UAcquireNPCsConditionProgress::IsComplete_Implementation() {
	return HaveAcquire;
}

void UAcquireNPCsConditionProgress::HandleComplete_Implementation() {
	UAcquireNPCsCondition* AcquireNPCsCondition = Cast<UAcquireNPCsCondition>(Condition);
	auto GameInstance = Cast<UCoreGameInstance>(ProgressOwner->GetWorld()->GetGameInstance());
	if (GameInstance) {
		auto NPCSystem = GameInstance->GameSystemManager->GetSystemByClass<UNPCSystem>();
		if (NPCSystem) {
			auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);

			NPCSystem->ReleaseNPCs(ConditionPlayerState, AcquireNPCsCondition->UnitIDs);
		}
	}
}

TArray<TSubclassOf<class UGameEventBase>> UAcquireNPCsConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			UNPCAcquireEvent::StaticClass(),
			UNPCReleaseEvent::StaticClass(),
		};
	}
}

void UAcquireNPCsConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	if (HaveAcquire) {
		return;
	}
	if (UNPCReleaseEvent* NPCReleaseEvent = Cast<UNPCReleaseEvent>(HandleEvent)) {
		//通知任务组件可以进行NPC占有逻辑
		auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
		auto EventPlayerState = UGameFrameworkUtils::GetEntityState(NPCReleaseEvent->Source);
		if (ConditionPlayerState == EventPlayerState) {
			if (ConditionPlayerState->QuestComponent) {
				ConditionPlayerState->QuestComponent->NotifyAcquireNPCs();
			}
			return;
		}
	}
	else if (UNPCAcquireEvent* NPCAcquireEvent = Cast<UNPCAcquireEvent>(HandleEvent)) {
		auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
		auto EventPlayerState = UGameFrameworkUtils::GetEntityState(NPCAcquireEvent->Source);
		if (ConditionPlayerState == EventPlayerState) {
			if (NPCAcquireEvent->CustomInfo == this) {
				HaveAcquire = true;

				RefreshSatisfy();
			}
		}
	}
}

void UAcquireNPCsConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UAcquireNPCsConditionProgress, HaveAcquire, Params);
}