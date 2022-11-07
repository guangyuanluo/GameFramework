// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioAction.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, config = Game)
class GAMEFRAMEWORK_API UScenarioAction : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Execute(class UScenario* Scenario, class UScenarioNode* ScenarioNode);

	UFUNCTION(BlueprintNativeEvent)
	FString GetNodeTitle();
};