// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestDetailNodeItem.h"
#include "CoreCondition.h"
#include "CoreReward.h"

UQuestDetailNodeItem::UQuestDetailNodeItem(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    
}

FString UQuestDetailNodeItem::GetNodeTitle_Implementation() {
    return Name;
}

FString UQuestDetailNodeItem::GetNodeTypeName_Implementation() {
    return TEXT("任务子项");
}

#if WITH_EDITOR

void UQuestDetailNodeItem::PostEditImport() {
    auto Outer = GetOuter();
    if (Conditions.Num() > 0) {
        TArray<UCoreCondition*> NewConditions;
        for (auto Condition : Conditions) {
            auto NewCondition = DuplicateObject(Condition, Outer);
            NewConditions.Add(NewCondition);
        }
        Conditions = NewConditions;
    }

    if (Rewards.Num() > 0) {
        TArray<UCoreReward*> NewRewards;
        for (auto Reward : Rewards) {
            auto NewReward = DuplicateObject(Reward, Outer);
            NewRewards.Add(NewReward);
        }
        Rewards = NewRewards;
    }
}

void UQuestDetailNodeItem::PostDuplicate(bool bDuplicateForPIE) {
    PostEditImport();
}

#endif