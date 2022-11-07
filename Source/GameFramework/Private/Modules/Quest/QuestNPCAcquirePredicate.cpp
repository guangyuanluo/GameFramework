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
        // Aû���������������ȼ����Ǹߵ�
        return true;
    }
    //A����������
    bool IsBHaveOtherCondition = true;
    for (auto QuestProgress : A->GetQuestProgresses()) {
        if (!QuestProgress->IsA<UAcquireNPCsConditionProgress>()
            && !QuestProgress->IsA<UTalkToConditionProgress>()) {
            IsBHaveOtherCondition = false;
            break;
        }
    }
    if (IsBHaveOtherCondition) {
        //Bû����������
        return false;
    }
    //A B���������������ͱ���A��B��
   
    return true;
}