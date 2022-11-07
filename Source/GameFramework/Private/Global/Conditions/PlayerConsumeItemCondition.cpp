// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerConsumeItemCondition.h"
#include "PlayerConsumeItemConditionProgress.h"

UPlayerConsumeItemCondition::UPlayerConsumeItemCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerConsumeItemConditionProgress::StaticClass();
}