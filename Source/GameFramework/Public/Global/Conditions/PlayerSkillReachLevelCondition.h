// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Skill/SkillInfo.h"
#include "PlayerSkillReachLevelCondition.generated.h"

/**
 * 玩家技能达到等级条件
 */
UCLASS(BlueprintType, meta = (DisplayName = "技能达到等级", Category = "通用条件"))
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

/**
 * 玩家技能达到等级进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerSkillReachLevelConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	virtual bool IsComplete_Implementation(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/
};