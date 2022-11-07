// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Scenario/ScenarioNode.h"
#include "ScenarioNodeReturn.generated.h"

/**
 * 
 */
UCLASS()
class GAMEFRAMEWORK_API UScenarioNodeReturn : public UScenarioNode
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "返回分支"))
	int ReturnBranch = 0;

	virtual FString GetNodeTitle_Implementation() override;
	virtual TArray<FScenarioNodeOutputPinInfo> GetOutputPinInfo_Implementation() override;
	
};