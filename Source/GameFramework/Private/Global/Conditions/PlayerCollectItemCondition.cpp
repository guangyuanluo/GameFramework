// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCollectItemCondition.h"
#include "PlayerCollectItemConditionProgress.h"

UPlayerCollectItemCondition::UPlayerCollectItemCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerCollectItemConditionProgress::StaticClass();
}