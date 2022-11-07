// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestDetail.generated.h"

class UQuestDetailNode;

/**
 * 任务详情
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UQuestDetail : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Quest")
	UQuestDetailNode* Root;

	/**
	* 通过获取
	*/
	UFUNCTION(BlueprintPure)
	UQuestDetailNode* GetNodeByID(const FGuid& Guid);

#if WITH_EDITORONLY_DATA

	/** Graph for Scenario */
	UPROPERTY()
	class UEdGraph*	QuestDetailGraph;

	/** Info about the graphs we last edited */
	UPROPERTY()
	TArray<FEditedDocumentInfo> LastEditedDocuments;
#endif

	UPROPERTY(Transient)
	TMap<FGuid, UQuestDetailNode*> NodeMap;
};