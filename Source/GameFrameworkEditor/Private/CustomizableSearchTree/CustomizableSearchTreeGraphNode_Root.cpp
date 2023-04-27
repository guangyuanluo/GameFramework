// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CustomizableSearchTreeGraphNode_Root.h"
#include "UObject/UObjectIterator.h"
#include "CustomizableSearchTreeGraph.h"

UCustomizableSearchTreeGraphNode_Root::UCustomizableSearchTreeGraphNode_Root(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

bool UCustomizableSearchTreeGraphNode_Root::CanUserDeleteNode() const {
    return false;
}

void UCustomizableSearchTreeGraphNode_Root::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, PinCategory_MultipleNodes, TEXT("Next"));
}

FText UCustomizableSearchTreeGraphNode_Root::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("根"));
}

bool UCustomizableSearchTreeGraphNode_Root::AllowOutputPin(TSubclassOf<class UEdGraphNode> NodeClass) {
    return true;
}