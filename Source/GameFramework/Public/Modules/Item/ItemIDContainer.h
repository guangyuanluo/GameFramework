// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemIDContainer.generated.h"

/**
* 物品ID容器
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FItemIDContainer
{
	GENERATED_BODY()

	/**
	*  物品ID
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "物品ID"))
	int32 ItemID;
};