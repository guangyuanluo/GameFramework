// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerIntimacyRequestCondition.h"
#include "PlayerIntimacyRequestConditionProgress.h"

UPlayerIntimacyRequestCondition::UPlayerIntimacyRequestCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerIntimacyRequestConditionProgress::StaticClass();
}