// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreCondition.h"
#include "CoreConditionProgress.h"

UCoreCondition::UCoreCondition(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    
}

class UCoreConditionProgress* UCoreCondition::GenerateConditionProgress(AActor* ProgressOwner) {
    auto ConditionProgress = NewObject<UCoreConditionProgress>(ProgressOwner, ProgressClass);
    if (ConditionProgress) {
        ConditionProgress->Condition = this;
        ConditionProgress->ProgressOwner = ProgressOwner;

        ConditionProgress->PostProgressInitialize();
    }
    return ConditionProgress;
}