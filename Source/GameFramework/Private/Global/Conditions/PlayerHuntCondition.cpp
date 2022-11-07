// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHuntCondition.h"
#include "PlayerHuntConditionProgress.h"

UPlayerHuntCondition::UPlayerHuntCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerHuntConditionProgress::StaticClass();
}