// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "MoveInputInTimeCondition.generated.h"

/**
 * 有移动输入
 */
UCLASS(BlueprintType, meta = (DisplayName = "当前有移动输入", Category = "通用条件"))
class GAMEFRAMEWORK_API UMoveInputInTimeCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 多少时间内，单位毫秒
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	float InTime = 200.0f;
};

/**
 * 有移动进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UMoveInputInTimeConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	virtual bool IsComplete(bool& IsValid) override;
};