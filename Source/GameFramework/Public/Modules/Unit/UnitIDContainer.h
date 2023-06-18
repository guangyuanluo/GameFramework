// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitIDContainer.generated.h"

/**
* 单位ID容器
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FUnitIDContainer
{
	GENERATED_BODY()

	/**
	*  单位ID
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "单位ID"))
	int32 UnitID;
};