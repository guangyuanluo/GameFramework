// Fill out your copyright notice in the Description page of Project Settings.

#include "MatchUnitTemplateCondition.h"
#include "CoreCharacter.h"
#include "CoreCharacterStateBase.h"
#include "CoreGameInstance.h"
#include "ExecutingQuest.h"

UMatchUnitTemplateCondition::UMatchUnitTemplateCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UMatchUnitTemplateConditionProgress::StaticClass();
}

bool UMatchUnitTemplateConditionProgress::IsComplete(bool& IsValid) {
	IsValid = false;
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (!ConditionPlayerState) {
		return false;
	}
	auto Character = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());
	if (!Character) {
		return false;
	}
	IsValid = true;
	auto ThisCondition = Cast<UMatchUnitTemplateCondition>(Condition);
	if (Character->TemplateID != ThisCondition->UnitIDContainer.UnitID) {
		return false;
	}
	return true;
}