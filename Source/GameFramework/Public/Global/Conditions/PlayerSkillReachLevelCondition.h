// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Skill/SkillInfo.h"
#include "PlayerSkillReachLevelCondition.generated.h"

/**
 * 玩家技能达到等级条件
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerSkillReachLevelCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 技能信息
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FSkillInfo Skill;
};
