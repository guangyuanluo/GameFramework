// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GraphEditorBase.h"

class UScenario;

/**
 * 
 */
class FQuestDetailEditor : public FGraphEditorBase
{
public:
	/**
	* Get the localized text to display for the specified mode
	* @param	InMode	The mode to display
	* @return the localized text representation of the mode
	*/
	static FText GetLocalizedMode(FName InMode);
public:
	virtual FName GetAppIdentifier() override;	
	virtual TSharedRef<FApplicationMode> GetApplicationMode() override;
	virtual UGameFrameworkGraph* GetGraph() override;
	virtual void OnSetGraph(UGameFrameworkGraph* InGraph) override;
	virtual FName GetGraphName() const override;
	virtual TSubclassOf<class UEdGraph> GetGraphClass() override;
	virtual TSubclassOf<class UEdGraphSchema> GetSchemaClass() override;
	virtual TArray<FEditedDocumentInfo>& GetEditDocumentInfo() override;

public:
	/** Modes in mode switcher */
	static const FName QuestDetailMode;
};