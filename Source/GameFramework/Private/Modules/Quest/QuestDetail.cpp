// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestDetail.h"
#include "QuestDetailNode.h"

UQuestDetail::UQuestDetail(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

UQuestDetailNode* UQuestDetail::GetNodeByID(const FGuid& Guid) {
    if (NodeMap.Num() == 0) {
        //缓存任务树对应的任务
        TQueue<UQuestDetailNode*> VisitNodes;
        if (Root) {
            VisitNodes.Enqueue(Root);
        }
        while (!VisitNodes.IsEmpty()) {
            UQuestDetailNode* VisitNode;
            VisitNodes.Dequeue(VisitNode);
            NodeMap.Add(VisitNode->ID, VisitNode);
            for (int NextIndex = 0; NextIndex < VisitNode->NextNodes.Num(); ++NextIndex) {
                auto NextNode = VisitNode->NextNodes[NextIndex];
                if (NextNode && !NodeMap.Contains(NextNode->ID)) {
                    VisitNodes.Enqueue(NextNode);
                }
            }
        }
    }
    auto FindNodePtr = NodeMap.Find(Guid);
    if (FindNodePtr) {
        return *FindNodePtr;
    }
    return nullptr;
}