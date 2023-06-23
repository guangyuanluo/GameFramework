// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitReachExpLevelConditionProgress.h"
#include "UnitReachExpLevelCondition.h"
#include "ExpLevelUpEvent.h"
#include "CoreCharacter.h"
#include "CorePlayerController.h"

void UUnitReachExpLevelConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UUnitReachExpLevelConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			UExpLevelUpEvent::StaticClass(),
		};
	}
}

void UUnitReachExpLevelConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UExpLevelUpEvent* ExpLevelUpEvent = (UExpLevelUpEvent*)HandleEvent;
	ACoreCharacter* Unit = Cast<ACoreCharacter>(ExpLevelUpEvent->Source);
	if (Unit == nullptr) {
		return;
	}
	UUnitReachExpLevelCondition* ReachExpLevelCondition = (UUnitReachExpLevelCondition*)Condition;
	if (ExpLevelUpEvent->ExpTypeId == ReachExpLevelCondition->ExpTypeContainer.ExpType
		&& Unit->TemplateID == ReachExpLevelCondition->UnitIDContainer.UnitID) {
		RefreshSatisfy();
	}
}

bool UUnitReachExpLevelConditionProgress::IsComplete_Implementation() {
	return false;
}

void UUnitReachExpLevelConditionProgress::HandleComplete_Implementation() {

}