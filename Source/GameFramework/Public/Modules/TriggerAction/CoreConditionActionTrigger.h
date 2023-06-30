// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionList.h"
#include "Modules/TriggerAction/CoreTriggerActionList.h"
#include "Modules/Condition/ConditionTriggerHandler.h"
#include "CoreConditionActionTrigger.generated.h"

/**
 * 条件动作触发器配置
 */
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FCoreConditionActionTriggerConfig
{
	GENERATED_BODY()

	/**
	* 触发条件
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "触发条件"))
	FCoreConditionList TriggerConditions;

	/**
	* 执行动作
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "执行动作"))
	FCoreTriggerActionList ExecuteActions;
};

/**
 * 条件动作触发器
 */
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FCoreConditionActionTriggerInfo {
	GENERATED_BODY()

	/**
	* 触发条件进度
	*/
	UPROPERTY()
	TArray<class UCoreConditionProgress*> TriggerConditionProgresses;

	/**
	* 句柄
	*/
	struct FConditionTriggerHandler ConditionTriggerHandler;
};