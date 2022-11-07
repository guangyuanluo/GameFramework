// Fill out your copyright notice in the Description page of Project Settings.

#include "ArrivingAtCondition.h"
#include "ArrivingAtConditionProgress.h"

UArrivingAtCondition::UArrivingAtCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UArrivingAtConditionProgress::StaticClass();
}