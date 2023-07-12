// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "PlayerQuestFinishCondition.generated.h"

/**
 * 玩家任务完成条件
 */
UCLASS(BlueprintType, meta = (DisplayName = "任务完成", Category = "任务条件"))
class GAMEFRAMEWORK_API UPlayerQuestFinishCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 完成任务id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 FinishQuestId;
};
