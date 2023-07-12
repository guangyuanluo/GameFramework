// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "TalkToCondition.generated.h"

/**
 * NPC谈话
 */
UCLASS(BlueprintType, meta = (DisplayName = "NPC谈话", Category = "任务条件"))
class GAMEFRAMEWORK_API UTalkToCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* @brief 与哪个npc对话
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FUnitIDContainer UnitIDContainer;
};
