// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "CloseToNPCCondition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UCloseToNPCCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* @brief 靠近哪个npc
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int UnitID;

	/**
	* @brief 半径
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	float Radius;
};
