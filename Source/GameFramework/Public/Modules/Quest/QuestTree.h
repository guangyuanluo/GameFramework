// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTree.generated.h"

/**
 * 任务树
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UQuestTree : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	/**
	* id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quest", meta = (DisplayName = "任务树Id"))
	FGuid ID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Quest")
	class UQuest* Root;

#if WITH_EDITORONLY_DATA

	/** Graph for Scenario */
	UPROPERTY()
	class UEdGraph*	QuestEdGraph;

	/** Info about the graphs we last edited */
	UPROPERTY()
	TArray<FEditedDocumentInfo> LastEditedDocuments;
#endif
};