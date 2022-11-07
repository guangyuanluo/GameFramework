// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitReachExpLevelCondition.h"
#include "UnitReachExpLevelConditionProgress.h"

UUnitReachExpLevelCondition::UUnitReachExpLevelCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UUnitReachExpLevelConditionProgress::StaticClass();
}