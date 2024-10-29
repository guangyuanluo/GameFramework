// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest.h"
#include "CoreCondition.h"

UQuest::UQuest(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

#if WITH_EDITOR

void UQuest::PostEditImport() {
    if (PreConditionList.Conditions.Num() > 0) {
        bool bPreConditionDirty = false;
        TArray<UCoreCondition*> NewConditions;
        for (auto Condition : PreConditionList.Conditions) {
            if (Condition && Condition->GetOuter() != this) {
                bPreConditionDirty = true;
                auto NewCondition = DuplicateObject(Condition, this);
                NewConditions.Add(NewCondition);
            }
        }
        if (bPreConditionDirty) {
            PreConditionList.Conditions = NewConditions;
        }
    }
}

void UQuest::PostDuplicate(bool bDuplicateForPIE) {
    Super::PostDuplicate(bDuplicateForPIE);

    ID = FGuid::NewGuid();

    PostEditImport();
}

void UQuest::PostRename(UObject* OldOuter, const FName OldName) {
    Super::PostRename(OldOuter, OldName);

    for (auto Condition : PreConditionList.Conditions) {
        if (Condition) {
            Condition->Rename(nullptr, this, REN_ForceNoResetLoaders | REN_DoNotDirty | REN_DontCreateRedirectors | REN_NonTransactional);
        }
    }
}

void UQuest::PostLoad() {
    Super::PostLoad();

    PostEditImport();
}

#endif