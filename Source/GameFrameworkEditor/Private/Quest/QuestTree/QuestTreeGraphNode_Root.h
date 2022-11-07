// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestTreeGraphNode.h"
#include "QuestTreeGraphNode_Root.generated.h"

/** Root node of this graphtree */
UCLASS()
class UQuestTreeGraphNode_Root : public UQuestTreeGraphNode
{
	GENERATED_UCLASS_BODY()

public:
    virtual bool CanUserDeleteNode() const override;
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual bool AllowOutputPin(TSubclassOf<class UEdGraphNode> NodeClass) override;

};
