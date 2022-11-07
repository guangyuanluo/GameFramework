// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "AsyncDelay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelayExecuteCompleteDelegate);

/**
* “Ï≤Ω—”≥Ÿ
*/
UCLASS()
class GAMEFRAMEWORK_API UAsyncDelay : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=( BlueprintInternalUseOnly="true", WorldContext = "WorldContextObject"))
	static UAsyncDelay* DelayExecute(UObject* WorldContextObject, float DelayTime);

public:
	UPROPERTY(BlueprintAssignable)
	FDelayExecuteCompleteDelegate OnComplete;

	void Start(float DelayTime);

private:
	struct FTimerHandle TimerHandle;

	UFUNCTION()
	void TimerCallback();
};
