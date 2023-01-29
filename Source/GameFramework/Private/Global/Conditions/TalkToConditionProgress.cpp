// Fill out your copyright notice in the Description page of Project Settings.

#include "TalkToConditionProgress.h"
#include "TalkToCondition.h"
#include "CoreCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "TalkToNPCEvent.h"
#include "CoreCharacterStateBase.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "ConditionSystem.h"
#include "ExecutingQuest.h"

void UTalkToConditionProgress::PostProgressInitialize_Implementation() {
	
}

bool UTalkToConditionProgress::IsQuestOtherProgressesComplete() {
	auto GameInstance = Cast<UCoreGameInstance>(ProgressOwner->GetGameInstance());
	if (GameInstance) {
		auto ConditionSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionSystem>();
		if (ConditionSystem) {
			auto Observer = ConditionSystem->GetObserverFromProgress(this);
			auto ExecutingQuest = Cast<UExecutingQuest>(Observer.GetObject());
			bool IsQuestOtherProgressesComplete = true;
			for (auto Progress : ExecutingQuest->GetQuestProgresses()) {
				if (Progress != this && !Progress->IsComplete()) {
					IsQuestOtherProgressesComplete = false;
					break;
				}
			}
			return IsQuestOtherProgressesComplete;
		}
	}
	return false;
}

TArray<TSubclassOf<class UGameEventBase>> UTalkToConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UTalkToNPCEvent::StaticClass(),
		});
	}
}

bool UTalkToConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	if (HaveTalk) {
		return false;
	}
	if (UTalkToNPCEvent* TalkToEvent = Cast<UTalkToNPCEvent>(GameEvent)) {
		auto TalkToCondition = Cast<UTalkToCondition>(Condition);
		if (TalkToCondition->UnitID == TalkToEvent->NPCID) {
			auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
			auto ConditionCharacter = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());
			if (ConditionCharacter->GetEntityID() == TalkToEvent->EntityId) {
				if (IsQuestOtherProgressesComplete()) {
					HaveTalk = true;
					return true;
				}
			}
		}
	}
	return false;
}

bool UTalkToConditionProgress::IsComplete_Implementation() {
	return HaveTalk;
}

void UTalkToConditionProgress::HandleComplete_Implementation() {

}

void UTalkToConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTalkToConditionProgress, HaveTalk);
}