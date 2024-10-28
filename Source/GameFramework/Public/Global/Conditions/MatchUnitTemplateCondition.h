// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "MatchUnitTemplateCondition.generated.h"

/**
 * 匹配单位模板
 */
UCLASS(BlueprintType, meta = (DisplayName = "匹配单位模板", Category = "通用条件"))
class GAMEFRAMEWORK_API UMatchUnitTemplateCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* @brief 模板id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FUnitIDContainer UnitIDContainer;
};

/**
 * 匹配单位模板进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UMatchUnitTemplateConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	virtual bool IsComplete(bool& IsValid) override;
};