// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomizableSearchTreeNodeChecker.generated.h"

UCLASS(Blueprintable, BlueprintType, Abstract)
class GAMEFRAMEWORK_API UCustomizableSearchTreeNodeChecker : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 子类可以指定搜索方法
	 */
	UFUNCTION(BlueprintNativeEvent)
	class UCustomizableSearchTreeNodeBase* FindNode(class UCustomizableSearchTreeNodeBase* Node, TSubclassOf<class UCustomizableSearchTreeNodeBase> NodeClass, UObject* FindContext);

	UFUNCTION(BlueprintNativeEvent)
	bool NeedSearchChildren(class UCustomizableSearchTreeNodeBase* Node, UObject* FindContext);
};