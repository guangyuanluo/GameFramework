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

TArray<TSubclassOf<class UGameEventBase>> UAcquireNPCsConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UNPCAcquireEvent::StaticClass(),
			UNPCReleaseEvent::StaticClass(),
		});
	}
}

bool UAcquireNPCsConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	if (HaveAcquire) {
		return false;
	}
	if (UNPCReleaseEvent* NPCReleaseEvent = Cast<UNPCReleaseEvent>(GameEvent)) {
		//通知任务组件可以进行NPC占有逻辑
		auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
		auto EventPlayerState = UGameFrameworkUtils::GetEntityState(NPCReleaseEvent->Source);
		if (ConditionPlayerState == EventPlayerState) {
			if (ConditionPlayerState->QuestComponent) {
				ConditionPlayerState->QuestComponent->NotifyAcquireNPCs();
			}
			return true;
		}
	}
	else if (UNPCAcquireEvent* NPCAcquireEvent = Cast<UNPCAcquireEvent>(GameEvent)) {
		auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
		auto EventPlayerState = UGameFrameworkUtils::GetEntityState(NPCAcquireEvent->Source);
		if (ConditionPlayerState == EventPlayerState) {
			if (NPCAcquireEvent->CustomInfo == this) {
				HaveAcquire = true;
			}
			return true;
		}
	}
	return false;
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

void UAcquireNPCsConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAcquireNPCsConditionProgress, HaveAcquire);
}