// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCollectMoneyCondition.h"
#include "PlayerCollectMoneyConditionProgress.h"

UPlayerCollectMoneyCondition::UPlayerCollectMoneyCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerCollectMoneyConditionProgress::StaticClass();
}