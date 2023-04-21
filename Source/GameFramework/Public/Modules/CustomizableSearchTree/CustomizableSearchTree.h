// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomizableSearchTree.generated.h"

/**
 * 定制化动画搜索树
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UCustomizableSearchTree : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	/**
	* 树根节点
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	class UCustomizableSearchTreeNodeBase* RootScenario;

	/**
	* 限制树节点子类的类型
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	TSubclassOf<UCustomizableSearchTreeNodeBase> LimitNodeClass;

#if WITH_EDITORONLY_DATA

	/** Graph for Asset */
	UPROPERTY()
	class UEdGraph*	Graph;

	/** Info about the graphs we last edited */
	UPROPERTY()
	TArray<FEditedDocumentInfo> LastEditedDocuments;

#endif
};