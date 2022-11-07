// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ScenarioGraphNode_Root.h"
#include "UObject/UObjectIterator.h"
#include "ScenarioGraph.h"

UScenarioGraphNode_Root::UScenarioGraphNode_Root(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

bool UScenarioGraphNode_Root::CanUserDeleteNode() const {
    return false;
}

void UScenarioGraphNode_Root::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, PinCategory_SingleNode, TEXT("Next"));
}

FText UScenarioGraphNode_Root::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("根"));
}

bool UScenarioGraphNode_Root::AllowOutputPin(TSubclassOf<class UEdGraphNode> NodeClass) {
    return true;
}