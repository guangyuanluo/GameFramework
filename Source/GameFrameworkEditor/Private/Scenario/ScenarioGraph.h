// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graph/GameFrameworkGraph.h"
#include "ScenarioGraph.generated.h"

class UGameFrameworkGraphNode;

UCLASS()
class UScenarioGraph : public UGameFrameworkGraph
{
	GENERATED_UCLASS_BODY()

protected:
	/** Signal to listeners that the graph has changed */
	virtual void NotifyGraphPinConnectionChanged() override;
private:
	void UpdateScenarioNodeRecusive(UGameFrameworkGraphNode* node);
};
