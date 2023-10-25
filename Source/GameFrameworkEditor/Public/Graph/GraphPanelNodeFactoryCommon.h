// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

class GAMEFRAMEWORKEDITOR_API FGraphPanelNodeFactoryCommon : public FGraphPanelNodeFactory {
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};