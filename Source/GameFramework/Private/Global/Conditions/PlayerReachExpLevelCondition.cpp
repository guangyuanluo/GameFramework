// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerReachExpLevelCondition.h"
#include "PlayerReachExpLevelConditionProgress.h"

UPlayerReachExpLevelCondition::UPlayerReachExpLevelCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerReachExpLevelConditionProgress::StaticClass();
}