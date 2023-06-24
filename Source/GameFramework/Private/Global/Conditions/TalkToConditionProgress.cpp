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

bool UTalkToConditionProgress::IsComplete_Implementation() {
	return HaveTalk;
}

TArray<TSubclassOf<class UGameEventBase>> UTalkToConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			UTalkToNPCEvent::StaticClass(),
		};
	}
}

void UTalkToConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	if (HaveTalk) {
		return;
	}
	if (UTalkToNPCEvent* TalkToEvent = Cast<UTalkToNPCEvent>(HandleEvent)) {
		auto TalkToCondition = Cast<UTalkToCondition>(Condition);
		if (TalkToCondition->UnitIDContainer.UnitID == TalkToEvent->NPCID) {
			auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
			auto ConditionCharacter = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());
			if (ConditionCharacter->GetEntityID() == TalkToEvent->EntityId) {
				if (IsQuestOtherProgressesComplete()) {
					HaveTalk = true;

					RefreshSatisfy();
				}
			}
		}
	}
}

void UTalkToConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UTalkToConditionProgress, HaveTalk, Params);
}