// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "ArrivingAtCondition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, meta = (DisplayName = "到达地点"))
class GAMEFRAMEWORK_API UArrivingAtCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* @brief 指定到达的单位id，即UnitInfo的id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FUnitIDContainer UnitIDContainer;

	/**
	* @brief 得到移动到的名字
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FString LocationName;
};
