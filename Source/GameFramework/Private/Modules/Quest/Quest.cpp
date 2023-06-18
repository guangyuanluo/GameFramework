// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest.h"
#include "CoreCondition.h"

UQuest::UQuest(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

#if WITH_EDITOR

void UQuest::PostEditImport() {
    if (PreConditionList.Conditions.Num() > 0) {
        auto Outer = GetOuter();
        TArray<UCoreCondition*> NewConditions;
        for (auto Condition : PreConditionList.Conditions) {
            auto NewCondition = DuplicateObject(Condition, Outer);
            NewConditions.Add(NewCondition);
        }
        PreConditionList.Conditions = NewConditions;
    }
}

void UQuest::PostDuplicate(bool bDuplicateForPIE) {
    Super::PostDuplicate(bDuplicateForPIE);

    ID = FGuid::NewGuid();

    PostEditImport();
}

#endif