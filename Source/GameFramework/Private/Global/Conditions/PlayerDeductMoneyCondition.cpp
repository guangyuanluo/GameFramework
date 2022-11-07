// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerDeductMoneyCondition.h"
#include "PlayerDeductMoneyConditionProgress.h"

UPlayerDeductMoneyCondition::UPlayerDeductMoneyCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerDeductMoneyConditionProgress::StaticClass();
}