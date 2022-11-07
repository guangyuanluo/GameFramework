// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graph/GameFrameworkGraphNode.h"
#include "QuestDetailGraphNode.generated.h"

/** Base node of this scenario */
UCLASS()
class UQuestDetailGraphNode : public UGameFrameworkGraphNode
{
	GENERATED_UCLASS_BODY()

public:
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;
    virtual void RefreshNodeOnDataChanged() override;
    virtual void InitializeInstance() override;
};
