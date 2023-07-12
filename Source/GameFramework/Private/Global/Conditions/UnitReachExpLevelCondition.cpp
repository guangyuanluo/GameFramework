// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitReachExpLevelCondition.h"
#include "ExpLevelUpEvent.h"
#include "CoreCharacter.h"
#include "CorePlayerController.h"

UUnitReachExpLevelCondition::UUnitReachExpLevelCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UUnitReachExpLevelConditionProgress::StaticClass();
}

TArray<TSubclassOf<class UGameEventBase>> UUnitReachExpLevelConditionProgress::GetHandleEventTypes_Implementation() {
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

bool UUnitReachExpLevelConditionProgress::IsComplete_Implementation(bool& IsValid) {
	IsValid = true;
	return false;
}