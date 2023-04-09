// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreCondition.h"
#include "CoreConditionProgress.h"
#include "ConditionStoreSubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

UCoreCondition::UCoreCondition(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    if (ID.IsValid()) {
        UConditionStoreSubsystem* ConditionStoreSubsystem = Cast<UConditionStoreSubsystem>(USubsystemBlueprintLibrary::GetGameInstanceSubsystem(this, UConditionStoreSubsystem::StaticClass()));

        if (ConditionStoreSubsystem) {
            ConditionStoreSubsystem->RegistCondition(this);
        }
    }
}

void UCoreCondition::GenerateGuid() {
    if (!ID.IsValid()) {
        ID = FGuid::NewGuid();
        UConditionStoreSubsystem* ConditionStoreSubsystem = Cast<UConditionStoreSubsystem>(USubsystemBlueprintLibrary::GetGameInstanceSubsystem(this, UConditionStoreSubsystem::StaticClass()));

        if (ConditionStoreSubsystem) {
            ConditionStoreSubsystem->RegistCondition(this);
        }
    }
}

class UCoreConditionProgress* UCoreCondition::GenerateConditionProgress(AActor* ProgressOwner) {
    auto ConditionProgress = NewObject<UCoreConditionProgress>(ProgressOwner, ProgressClass);
    if (ConditionProgress) {
        ConditionProgress->Condition = this;
        ConditionProgress->ConditionID = ID;
        ConditionProgress->ProgressOwner = ProgressOwner;

        ConditionProgress->PostProgressInitialize();
    }
    return ConditionProgress;
}