// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Item/ItemIDNumPair.h"
#include "PlayerDeductItemCondition.generated.h"

/**
 * 玩家扣除物品
 */
UCLASS(BlueprintType, meta = (DisplayName = "玩家扣除物品", Category = "任务条件"))
class GAMEFRAMEWORK_API UPlayerDeductItemCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 物品组
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "扣除物品"))
	TArray<FItemIDNumPair> Items;
};
