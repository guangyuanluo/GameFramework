// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "QuestTreeGraphNode_Root.h"
#include "UObject/UObjectIterator.h"
#include "ScenarioGraph.h"

UQuestTreeGraphNode_Root::UQuestTreeGraphNode_Root(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

bool UQuestTreeGraphNode_Root::CanUserDeleteNode() const {
    return false;
}

void UQuestTreeGraphNode_Root::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, PinCategory_SingleNode, TEXT("Next"));
}

FText UQuestTreeGraphNode_Root::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("开始"));
}

bool UQuestTreeGraphNode_Root::AllowOutputPin(TSubclassOf<class UEdGraphNode> NodeClass) {
    return true;
}