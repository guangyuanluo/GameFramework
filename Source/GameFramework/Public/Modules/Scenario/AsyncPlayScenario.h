// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "AsyncPlayScenario.generated.h"

class UScenario;
class UScenarioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayScenarioDelegate, UScenario*, Scenario, int, ReturnBranch);

/**
* 播放剧情的异步函数
*/
UCLASS()
class GAMEFRAMEWORK_API UAsyncPlayScenario : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=( BlueprintInternalUseOnly="true", WorldContext = "WorldContextObject"))
	static UAsyncPlayScenario* PlayScenario(UObject* WorldContextObject, UScenarioComponent* ScenarioComponent, UScenario* InScenario);

public:

	UPROPERTY(BlueprintAssignable)
    FPlayScenarioDelegate OnComplete;

    UPROPERTY(BlueprintReadOnly)
    UScenario* Scenario;
};
