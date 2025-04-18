// Fill out your copyright notice in the Description page of Project Settings.

#include "TalkToCondition.h"
#include "CoreCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "TalkToNPCEvent.h"
#include "CoreCharacterStateBase.h"
#include "CoreGameInstance.h"
#include "ExecutingQuest.h"

UTalkToCondition::UTalkToCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UTalkToConditionProgress::StaticClass();
}

bool UTalkToConditionProgress::IsComplete(bool& IsValid) {
	IsValid = true;
	return HaveTalk;
}

TArray<TSubclassOf<class UGameEventBase>> UTalkToConditionProgress::GetHandleEventTypes() {
	bool IsValid;
	bool bComplete = IsComplete(IsValid);
	if (!IsValid) {
		return {};
	}
	if (bComplete) {
		return {};
	}
	else {
		return {
			UTalkToNPCEvent::StaticClass(),
		};
	}
}

void UTalkToConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	if (HaveTalk) {
		return;
	}
	if (UTalkToNPCEvent* TalkToEvent = Cast<UTalkToNPCEvent>(HandleEvent)) {
		auto TalkToCondition = Cast<UTalkToCondition>(Condition);
		if (TalkToCondition->UnitIDContainer.UnitID == TalkToEvent->NPCID) {
			auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
			auto ConditionCharacter = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());
			if (ConditionCharacter->GetEntityID() == TalkToEvent->EntityId) {
				HaveTalk = true;

				RefreshSatisfy();

				HaveTalk = false;
				RefreshSatisfy();
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