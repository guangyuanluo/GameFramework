// Fill out your copyright notice in the Description page of Project Settings.

#include "AcquireNPCsCondition.h"
#include "AcquireNPCsConditionProgress.h"

UAcquireNPCsCondition::UAcquireNPCsCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UAcquireNPCsConditionProgress::StaticClass();
}