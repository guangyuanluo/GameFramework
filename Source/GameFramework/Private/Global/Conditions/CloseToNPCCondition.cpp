// Fill out your copyright notice in the Description page of Project Settings.

#include "CloseToNPCCondition.h"
#include "CloseToNPCConditionProgress.h"

UCloseToNPCCondition::UCloseToNPCCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UCloseToNPCConditionProgress::StaticClass();
}