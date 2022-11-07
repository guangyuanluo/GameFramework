// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "QuestDetailGraphNode.h"
#include "Modules/Quest/QuestDetailNode.h"

UQuestDetailGraphNode::UQuestDetailGraphNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

FText UQuestDetailGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    UQuestDetailNode* QuestDetailNode = Cast<UQuestDetailNode>(NodeInstance);
    FString ThisNodeTitle = QuestDetailNode->GetNodeTitle();
    if (ThisNodeTitle.IsEmpty()) {
        ThisNodeTitle = QuestDetailNode->GetNodeTypeName();
    }
    return FText::FromString(ThisNodeTitle);
}

void UQuestDetailGraphNode::AllocateDefaultPins() {
    CreatePin(EGPD_Input, PinCategory_MultipleNodes, TEXT("Parent"));
    CreatePin(EGPD_Output, PinCategory_MultipleNodes, TEXT("Next"));
}

void UQuestDetailGraphNode::RefreshNodeOnDataChanged() {
    
}

void UQuestDetailGraphNode::InitializeInstance() {
    Super::InitializeInstance();
    UQuestDetailNode* QuestDetailNode = Cast<UQuestDetailNode>(NodeInstance);
    QuestDetailNode->ID = FGuid::NewGuid();
}