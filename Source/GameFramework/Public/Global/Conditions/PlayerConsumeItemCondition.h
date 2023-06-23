// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Item/ItemIDContainer.h"
#include "PlayerConsumeItemCondition.generated.h"

/**
 * 玩家消耗物品
 */
UCLASS(BlueprintType, meta = (DisplayName = "玩家消耗物品"))
class GAMEFRAMEWORK_API UPlayerConsumeItemCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 物品id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FItemIDContainer ItemIDContainer;

	/**
	* 物品数量
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 ItemCount;
};
