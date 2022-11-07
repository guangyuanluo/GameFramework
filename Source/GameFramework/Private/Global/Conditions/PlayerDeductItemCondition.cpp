// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerDeductItemCondition.h"
#include "PlayerDeductItemConditionProgress.h"

UPlayerDeductItemCondition::UPlayerDeductItemCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerDeductItemConditionProgress::StaticClass();
}