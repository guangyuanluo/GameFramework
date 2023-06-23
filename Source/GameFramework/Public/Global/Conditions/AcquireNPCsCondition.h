// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "AcquireNPCsCondition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, meta=(DisplayName = "占有NPC"))
class GAMEFRAMEWORK_API UAcquireNPCsCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* @brief 占有NPC数组
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	TArray<FUnitIDContainer> UnitIDContainers;
};
