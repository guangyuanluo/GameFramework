// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "QuestTreeGraphNode.h"
#include "Modules/Quest/Quest.h"

UQuestTreeGraphNode::UQuestTreeGraphNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

FText UQuestTreeGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    UQuest* Quest = Cast<UQuest>(NodeInstance);
    FString QuestName = Quest->QuestName;
    if (QuestName.IsEmpty()) {
        QuestName = TEXT("任务");
    }
    return FText::FromString(QuestName);
}

void UQuestTreeGraphNode::AllocateDefaultPins() {
    CreatePin(EGPD_Input, PinCategory_MultipleNodes, TEXT("Parent"));
    CreatePin(EGPD_Output, PinCategory_MultipleNodes, TEXT("Next"));
}

void UQuestTreeGraphNode::RefreshNodeOnDataChanged() {
    
}

void UQuestTreeGraphNode::InitializeInstance() {
    Super::InitializeInstance();
    UQuest* Quest = Cast<UQuest>(NodeInstance);
    Quest->ID = FGuid::NewGuid();
}