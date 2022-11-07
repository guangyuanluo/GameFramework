// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerQuestFinishCondition.h"
#include "PlayerQuestFinishConditionProgress.h"

UPlayerQuestFinishCondition::UPlayerQuestFinishCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerQuestFinishConditionProgress::StaticClass();
}