// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomizableSearchTreeBlueprintLibrary.generated.h"

/**
 * Game-specific blueprint library
 * Most games will need to implement one or more blueprint function libraries to expose their native code to blueprints
 */
UCLASS()
class GAMEFRAMEWORK_API UCustomizableSearchTreeBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 从搜索树中找到对应类型的节点
	 */
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "NodeClass"))
	static class UCustomizableSearchTreeNodeBase* FindNodeFromAnimSearchTree(class UCustomizableSearchTree* CustomizableSearchTree, TSubclassOf<class UCustomizableSearchTreeNodeBase> NodeClass, UObject* FindContext);
};
