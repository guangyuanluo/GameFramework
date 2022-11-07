// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioType.h"
#include "Scenario.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UScenario : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Scenario")
	FString Brief;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Scenario")
	EScenarioType ScenarioType;

	/** root node of loaded tree */
	UPROPERTY(BlueprintReadOnly, Category = "Scenario")
	class UScenarioNode* RootScenario;

#if WITH_EDITORONLY_DATA

	/** Graph for Scenario */
	UPROPERTY()
	class UEdGraph*	ScenarioGraph;

	/** Info about the graphs we last edited */
	UPROPERTY()
	TArray<FEditedDocumentInfo> LastEditedDocuments;

#endif
};