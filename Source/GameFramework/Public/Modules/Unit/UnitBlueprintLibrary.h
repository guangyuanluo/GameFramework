// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "UnitBlueprintLibrary.generated.h"


/**
 * 单位公共函数
 */
UCLASS()
class GAMEFRAMEWORK_API UUnitBlueprintLibrary : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	static bool IsUnitIDContainersContainerID(const TArray<FUnitIDContainer>& UnitIDContainers, int32 UnitID);
};