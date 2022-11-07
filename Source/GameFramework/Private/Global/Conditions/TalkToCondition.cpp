// Fill out your copyright notice in the Description page of Project Settings.

#include "TalkToCondition.h"
#include "TalkToConditionProgress.h"

UTalkToCondition::UTalkToCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UTalkToConditionProgress::StaticClass();
}