// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveInputInTimeCondition.h"
#include "CoreCharacter.h"
#include "CoreCharacterStateBase.h"
#include "CoreGameInstance.h"
#include "ExecutingQuest.h"

UMoveInputInTimeCondition::UMoveInputInTimeCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UMoveInputInTimeConditionProgress::StaticClass();
}

bool UMoveInputInTimeConditionProgress::IsComplete(bool& IsValid) {
	IsValid = false;
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (!ConditionPlayerState) {
		return false;
	}
	auto ConditionCharacter = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());
	if (!ConditionCharacter) {
		return false;
	}
	IsValid = true;
	auto NowTime = FDateTime::Now();
	FVector LastMoveInputValue;
	FDateTime LastMoveTime;
	ConditionCharacter->GetLastMoveInputInfo(LastMoveInputValue, LastMoveTime);
	auto DiffTime = (NowTime - LastMoveTime).GetTotalMilliseconds();
	auto ThisCondition = Cast<UMoveInputInTimeCondition>(Condition);
	if (DiffTime < ThisCondition->InTime) {
		return true;
	}
	return false;
}