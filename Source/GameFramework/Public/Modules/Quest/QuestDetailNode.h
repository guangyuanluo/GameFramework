// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestDetailNode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, config = Game)
class GAMEFRAMEWORK_API UQuestDetailNode : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	* id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quest")
	FGuid ID;

	UPROPERTY()
	class UQuestDetailNode* PreNode;

	UPROPERTY()
	TArray<class UQuestDetailNode*> NextNodes;

	UFUNCTION(BlueprintNativeEvent)
	FString GetNodeTitle();

	UFUNCTION(BlueprintNativeEvent)
	FString GetNodeTypeName();
};