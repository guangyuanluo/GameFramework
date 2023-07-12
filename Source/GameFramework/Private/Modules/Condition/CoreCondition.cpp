// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreCondition.h"
#include "CoreConditionProgress.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "ConditionGlobalSystem.h"

UCoreCondition::UCoreCondition(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    
}

class UCoreConditionProgress* UCoreCondition::GenerateConditionProgress(AActor* ProgressOwner) {
    auto ConditionGlobalSystem = Cast<UConditionGlobalSystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(ProgressOwner, UConditionGlobalSystem::StaticClass()));
    return ConditionGlobalSystem->MallocProgress(ProgressOwner, this);
}