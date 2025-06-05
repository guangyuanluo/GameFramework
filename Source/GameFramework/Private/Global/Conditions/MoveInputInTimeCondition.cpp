// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Conditions/MoveInputInTimeCondition.h"
#include "Character/CoreCharacter.h"
#include "PlayerState/CoreCharacterStateBase.h"
#include "GameInstance/CoreGameInstance.h"
#include "Modules/Quest/ExecutingQuest.h"

UMoveInputInTimeCondition::UMoveInputInTimeCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UMoveInputInTimeConditionProgress::StaticClass();
}

EConditionSatisfy UMoveInputInTimeConditionProgress::GetSatisfy() {
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (!ConditionPlayerState) {
		return EConditionSatisfy::ECS_Unknown;
	}
	auto ConditionCharacter = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());
	if (!ConditionCharacter) {
		return EConditionSatisfy::ECS_Unknown;
	}
	auto NowTime = FDateTime::Now();
	FVector LastMoveInputValue;
	FDateTime LastMoveTime;
	ConditionCharacter->GetLastMoveInputInfo(LastMoveInputValue, LastMoveTime);
	auto DiffTime = (NowTime - LastMoveTime).GetTotalMilliseconds();
	auto ThisCondition = Cast<UMoveInputInTimeCondition>(Condition);
	if (DiffTime < ThisCondition->InTime) {
		return EConditionSatisfy::ECS_Satisfy;
	}
	return EConditionSatisfy::ECS_NotSatisfy;
}