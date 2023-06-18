// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "UnitReachExpLevelCondition.generated.h"

/**
 * 单位达到经验等级条件
 */
UCLASS(BlueprintType, meta = (DisplayName = "单位达到经验等级"))
class GAMEFRAMEWORK_API UUnitReachExpLevelCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 单位id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int UnitId;

	/**
	* 经验类型
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	uint8 ExpType;

	/**
	* 经验等级
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 ExpLevel;
};
