// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "PlayerFarmCondition.generated.h"

/**
 * 砍怪
 */
UCLASS(BlueprintType, meta = (DisplayName = "砍怪"))
class GAMEFRAMEWORK_API UPlayerFarmCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 单位id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FUnitIDContainer UnitIDContainer;

	/**
	* 单位数量
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 Count;
};
