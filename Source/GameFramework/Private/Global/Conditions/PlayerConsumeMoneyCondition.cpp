// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerConsumeMoneyCondition.h"
#include "PlayerConsumeMoneyConditionProgress.h"

UPlayerConsumeMoneyCondition::UPlayerConsumeMoneyCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerConsumeMoneyConditionProgress::StaticClass();
}