// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomizableSearchTreeNodeVisit.generated.h"

DECLARE_DELEGATE_ThreeParams(FCustomizableSearchTreeNodeVisitFunc, class UCustomizableSearchTreeNodeBase*, bool& /* NeedSearchChildren */, bool&/*NeedBreakLoop*/);

UCLASS()
class GAMEFRAMEWORK_API UCustomizableSearchTreeNodeVisit : public UObject
{
	GENERATED_BODY()

public:
	virtual void VisitNode(class UCustomizableSearchTreeNodeBase* Node, FCustomizableSearchTreeNodeVisitFunc VisitNodeFunc, bool& NeedSearchChildren, bool& NeedBreakLoop);
};