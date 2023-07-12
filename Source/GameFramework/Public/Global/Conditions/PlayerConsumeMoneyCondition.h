// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Money/MoneyTypeContainer.h"
#include "PlayerConsumeMoneyCondition.generated.h"

/**
 * 玩家消耗货币
 */
UCLASS(BlueprintType, meta = (DisplayName = "玩家消耗货币", Category = "任务条件"))
class GAMEFRAMEWORK_API UPlayerConsumeMoneyCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 金币类型
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FMoneyTypeContainer MoneyTypeContainer;

	/**
	* 金币数量
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 MoneyCount;
};
