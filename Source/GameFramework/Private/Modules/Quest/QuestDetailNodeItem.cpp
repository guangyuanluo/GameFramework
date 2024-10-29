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
    if (ConditionList.Conditions.Num() > 0) {
        bool bConditionDirty = false;
        TArray<UCoreCondition*> NewConditions;
        for (auto Condition : ConditionList.Conditions) {
            if (Condition && Condition->GetOuter() != this) {
                bConditionDirty = true;
                auto NewCondition = DuplicateObject(Condition, this);
                NewConditions.Add(NewCondition);
            }
        }
        if (bConditionDirty) {
            ConditionList.Conditions = NewConditions;
        }
    }

    if (Rewards.Num() > 0) {
        bool bRewardsDirty = false;
        TArray<UCoreReward*> NewRewards;
        for (auto Reward : Rewards) {
            if (Reward && Reward->GetOuter() != this) {
                bRewardsDirty = true;
                auto NewReward = DuplicateObject(Reward, this);
                NewRewards.Add(NewReward);
            }
        }
        if (bRewardsDirty) {
            Rewards = NewRewards;
        }
    }
}

void UQuestDetailNodeItem::PostDuplicate(bool bDuplicateForPIE) {
    PostEditImport();
}

void UQuestDetailNodeItem::PostRename(UObject* OldOuter, const FName OldName) {
    Super::PostRename(OldOuter, OldName);

    if (ConditionList.Conditions.Num() > 0) {
        for (auto Condition : ConditionList.Conditions) {
            if (Condition) {
                Condition->Rename(nullptr, this, REN_ForceNoResetLoaders | REN_DoNotDirty | REN_DontCreateRedirectors | REN_NonTransactional);
            }
        }
    }

    if (Rewards.Num() > 0) {
        for (auto Reward : Rewards) {
            if (Reward) {
                Reward->Rename(nullptr, this, REN_ForceNoResetLoaders | REN_DoNotDirty | REN_DontCreateRedirectors | REN_NonTransactional);
            }
        }
    }
}

void UQuestDetailNodeItem::PostLoad() {
    Super::PostLoad();

    PostEditImport();
}

#endif