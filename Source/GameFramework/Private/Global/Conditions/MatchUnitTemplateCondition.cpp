// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Conditions/MatchUnitTemplateCondition.h"
#include "Character/CoreCharacter.h"
#include "PlayerState/CoreCharacterStateBase.h"
#include "GameInstance/CoreGameInstance.h"
#include "Modules/Quest/ExecutingQuest.h"

UMatchUnitTemplateCondition::UMatchUnitTemplateCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UMatchUnitTemplateConditionProgress::StaticClass();
}

EConditionSatisfy UMatchUnitTemplateConditionProgress::GetSatisfy() {
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (!ConditionPlayerState) {
		return EConditionSatisfy::ECS_Unknown;
	}
	auto Character = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());
	if (!Character) {
		return EConditionSatisfy::ECS_Unknown;
	}
	auto ThisCondition = Cast<UMatchUnitTemplateCondition>(Condition);
	if (Character->TemplateID != ThisCondition->UnitIDContainer.UnitID) {
		return EConditionSatisfy::ECS_NotSatisfy;
	}
	return EConditionSatisfy::ECS_Satisfy;
}