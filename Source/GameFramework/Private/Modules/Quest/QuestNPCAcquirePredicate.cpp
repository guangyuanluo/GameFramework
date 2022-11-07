// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestNPCAcquirePredicate.h"
#include "QuestComponent.h"
#include "ExecutingQuest.h"
#include "AcquireNPCsConditionProgress.h"
#include "TalkToConditionProgress.h"

bool UQuestNPCAcquirePredicate::Compare_Implementation(UQuestComponent* QuestComponent, UExecutingQuest* A, UExecutingQuest* B) {
    bool IsAHaveOtherCondition = true;
    for (auto QuestProgress : A->GetQuestProgresses()) {
        if (!QuestProgress->IsA<UAcquireNPCsConditionProgress>()
            && !QuestProgress->IsA<UTalkToConditionProgress>()) {
            IsAHaveOtherCondition = false;
            break;
        }
    }

    if (IsAHaveOtherCondition) {
        // A没有其他条件，优先级就是高的
        return true;
    }
    //A有其他条件
    bool IsBHaveOtherCondition = true;
    for (auto QuestProgress : A->GetQuestProgresses()) {
        if (!QuestProgress->IsA<UAcquireNPCsConditionProgress>()
            && !QuestProgress->IsA<UTalkToConditionProgress>()) {
            IsBHaveOtherCondition = false;
            break;
        }
    }
    if (IsBHaveOtherCondition) {
        //B没有其他条件
        return false;
    }
    //A B都有其他条件，就保持A在B后
   
    return true;
}