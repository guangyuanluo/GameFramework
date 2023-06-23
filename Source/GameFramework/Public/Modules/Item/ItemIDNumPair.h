// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Item/ItemIDContainer.h"
#include "ItemIDNumPair.generated.h"

/**
* 物品ID数量对
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FItemIDNumPair
{
	GENERATED_USTRUCT_BODY()

	/**
	* 物品id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	FItemIDContainer ItemIDContainer;

	/**
	* 物品数量
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item", meta = (DisplayName = "物品数量"))
	int32 ItemNum;
};