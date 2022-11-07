// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraph.h"
#include "GameFrameworkGraph.generated.h"

UCLASS()
class UGameFrameworkGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

public:
	class FGraphEditorBase* Editor;

	/**
	* ���ݱ仯ˢ�����нڵ�
	*/
	virtual void RefreshAllNodesOnDataChanged();
	virtual void NotifyGraphPinConnectionChanged() { }
};
