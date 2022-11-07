// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "QuestDetailGraphNode_Root.h"
#include "UObject/UObjectIterator.h"
#include "ScenarioGraph.h"

UQuestDetailGraphNode_Root::UQuestDetailGraphNode_Root(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

bool UQuestDetailGraphNode_Root::CanUserDeleteNode() const {
    return false;
}

void UQuestDetailGraphNode_Root::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, PinCategory_SingleNode, TEXT("Next"));
}

FText UQuestDetailGraphNode_Root::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("开始"));
}

bool UQuestDetailGraphNode_Root::AllowOutputPin(TSubclassOf<class UEdGraphNode> NodeClass) {
    return true;
}