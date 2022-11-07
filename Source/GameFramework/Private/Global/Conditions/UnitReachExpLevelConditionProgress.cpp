// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitReachExpLevelConditionProgress.h"
#include "UnitReachExpLevelCondition.h"
#include "IncreaseExpLevelEvent.h"
#include "CoreCharacter.h"
#include "CorePlayerController.h"

void UUnitReachExpLevelConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UUnitReachExpLevelConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UIncreaseExpLevelEvent::StaticClass(),
		});
	}
}

bool UUnitReachExpLevelConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UIncreaseExpLevelEvent* IncreaseExpLevelEvent = (UIncreaseExpLevelEvent*)GameEvent;
	ACoreCharacter* Unit = Cast<ACoreCharacter>(IncreaseExpLevelEvent->Source);
	if (Unit == nullptr) {
		return false;
	}
	UUnitReachExpLevelCondition* ReachExpLevelCondition = (UUnitReachExpLevelCondition*)Condition;
	if (IncreaseExpLevelEvent->ExpTypeId == ReachExpLevelCondition->ExpType
		&& Unit->TemplateID == ReachExpLevelCondition->UnitId) {
		return true;
	}
	return false;
}

bool UUnitReachExpLevelConditionProgress::IsComplete_Implementation() {
	return false;
}

void UUnitReachExpLevelConditionProgress::HandleComplete_Implementation() {

}