// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSkillReachLevelCondition.h"
#include "PlayerSkillReachLevelConditionProgress.h"

UPlayerSkillReachLevelCondition::UPlayerSkillReachLevelCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerSkillReachLevelConditionProgress::StaticClass();
}