// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "MoveAndRotateAsyncTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveAndRotateCompleteDelegate);

/**
* 分帧移动和旋转
*/
UCLASS()
class GAMEFRAMEWORK_API UMoveAndRotateAsyncTask : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=( BlueprintInternalUseOnly="true", WorldContext = "WorldContextObject"))
	static UMoveAndRotateAsyncTask* MoveAndRotateAsync(class AActor* InActor, float InTransitionTime, const FVector& InTargetLocation, float InTargetYaw);

public:
	UPROPERTY(BlueprintAssignable)
	FMoveAndRotateCompleteDelegate OnComplete;

	void Start(class AActor* InActor, float InTransitionTime, const FVector& InTargetLocation, float InTargetYaw);

private:
	struct FTimerHandle TimerHandle;
	float PlayTime = 0;

	UPROPERTY()
	class AActor* Actor;

	UPROPERTY()
	float TransitionTime;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	float StartYaw;

	UPROPERTY()
	float TargetYaw;

	UFUNCTION()
	void TimerCallback();
};
