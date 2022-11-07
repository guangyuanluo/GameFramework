// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerFarmCondition.h"
#include "PlayerFarmConditionProgress.h"

UPlayerFarmCondition::UPlayerFarmCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerFarmConditionProgress::StaticClass();
}