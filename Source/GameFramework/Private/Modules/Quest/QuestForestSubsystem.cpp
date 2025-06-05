// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/Quest/QuestForestSubsystem.h"
#include "Modules/Quest/QuestTree.h"
#include "Modules/Quest/QuestSetting.h"
#include "Modules/Quest/Quest.h"

void UQuestForestSubsystem::Init_Implementation() {
    const UQuestSetting* QuestSetting = GetDefault<UQuestSetting>();
    auto& PreloadQuestTrees = QuestSetting->PreloadQuestTrees;
    for (int Index = 0; Index < PreloadQuestTrees.Num(); ++Index) {
        auto PreloadQuestTreeAsset = PreloadQuestTrees[Index].LoadSynchronous();
        if (PreloadQuestTreeAsset) {
            AddToForest(PreloadQuestTreeAsset);
        }
    }
}

void UQuestForestSubsystem::AddToForest(UQuestTree* InQuestTree) {
    if (!InQuestTree->ID.IsValid()) {
        return;
    }
    if (QuestTreeMap.Contains(InQuestTree->ID)) {
        return;
    }
    QuestTreeMap.Add(InQuestTree->ID, InQuestTree);

    //������������Ӧ������
    TQueue<UQuest*> VisitQuests;
    if (InQuestTree->Root) {
        VisitQuests.Enqueue(InQuestTree->Root);
    }
    while (!VisitQuests.IsEmpty()) {
        UQuest* VisitQuest;
        VisitQuests.Dequeue(VisitQuest);
        QuestMap.Add(VisitQuest->ID, VisitQuest);
        for (int FollowIndex = 0; FollowIndex < VisitQuest->FollowQuests.Num(); ++FollowIndex) {
            auto FollowQuest = VisitQuest->FollowQuests[FollowIndex];
            FollowQuest->PreQuests.AddUnique(VisitQuest);
            if (FollowQuest && !QuestMap.Contains(FollowQuest->ID)) {
                VisitQuests.Enqueue(FollowQuest);
            }
        }
    }
}

void UQuestForestSubsystem::RemoveFromForest(UQuestTree* InQuestTree) {
    if (QuestTreeMap.Remove(InQuestTree->ID) > 0) {
        //�Ƴ���������Ӧ�����񻺴�
        TQueue<UQuest*> VisitQuests;
        if (InQuestTree->Root) {
            VisitQuests.Enqueue(InQuestTree->Root);
        }
        while (!VisitQuests.IsEmpty()) {
            UQuest* VisitQuest;
            VisitQuests.Dequeue(VisitQuest);
            QuestMap.Remove(VisitQuest->ID);
            for (int FollowIndex = 0; FollowIndex < VisitQuest->FollowQuests.Num(); ++FollowIndex) {
                auto FollowQuest = VisitQuest->FollowQuests[FollowIndex];
                if (FollowQuest && !QuestMap.Contains(FollowQuest->ID)) {
                    VisitQuests.Enqueue(FollowQuest);
                }
            }
        }
    }
}

const TMap<FGuid, UQuestTree*>& UQuestForestSubsystem::GetAllQuestTreeMap() const {
    return QuestTreeMap;
}

UQuestTree* UQuestForestSubsystem::GetQuestTreeByID(const FGuid& ID) const {
    auto FindQuestTreePtr = QuestTreeMap.Find(ID);
    if (FindQuestTreePtr) {
        return *FindQuestTreePtr;
    }
    return nullptr;
}

UQuest* UQuestForestSubsystem::GetQuestByID(const FGuid& ID) const {
    auto FindQuestPtr = QuestMap.Find(ID);
    if (FindQuestPtr) {
        return *FindQuestPtr;
    }
    return nullptr;
}