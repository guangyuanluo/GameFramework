// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graph/GameFrameworkGraphNode.h"
#include "ScenarioGraphNode.generated.h"

/** Base node of this scenario */
UCLASS()
class UScenarioGraphNode : public UGameFrameworkGraphNode
{
	GENERATED_UCLASS_BODY()

public:
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AllocateDefaultPins() override;
    virtual void RefreshNodeOnDataChanged() override;
};
