// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitReachExpLevelCondition.h"
#include "ExpLevelUpEvent.h"
#include "CoreCharacter.h"
#include "CorePlayerController.h"

UUnitReachExpLevelCondition::UUnitReachExpLevelCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UUnitReachExpLevelConditionProgress::StaticClass();
}

TArray<TSubclassOf<class UGameEventBase>> UUnitReachExpLevelConditionProgress::GetHandleEventTypes() {
	EConditionSatisfy Satisfy = GetSatisfy();
	if (Satisfy == EConditionSatisfy::ECS_Satisfy) {
		return {};
	}
	else {
		return {
			UExpLevelUpEvent::StaticClass(),
		};
	}
}

void UUnitReachExpLevelConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UExpLevelUpEvent* ExpLevelUpEvent = (UExpLevelUpEvent*)HandleEvent;
	ACoreCharacter* Unit = Cast<ACoreCharacter>(ExpLevelUpEvent->Source);
	if (Unit == nullptr) {
		return;
	}
	UUnitReachExpLevelCondition* ReachExpLevelCondition = (UUnitReachExpLevelCondition*)Condition;
	if (ExpLevelUpEvent->ExpType == ReachExpLevelCondition->ExpType
		&& Unit->TemplateID == ReachExpLevelCondition->UnitIDContainer.UnitID) {
		RefreshSatisfy();
	}
}

EConditionSatisfy UUnitReachExpLevelConditionProgress::GetSatisfy() {
	return EConditionSatisfy::ECS_NotSatisfy;
}