// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Scenario/Scenario.h"
#include "ScenarioTalkItem.generated.h"

class UCoreQuest;
class ANPCCharacter;

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UScenarioTalkItem : public UObject
{
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	int QuestTemplateId;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	bool Accept;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	bool IsComplete;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSoftObjectPtr<UScenario> Scenario;

	UFUNCTION(BlueprintCallable, Category = "UI")
	UScenarioNode* GetScenarioRootNode();
};