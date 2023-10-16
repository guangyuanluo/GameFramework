// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/Structure/LogicObjectBase.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "CoreTriggerAction.generated.h"

/**
 * 触发器动作
 */
UCLASS(BlueprintType, Blueprintable, Abstract, meta = (DisplayName = "动作"))
class GAMEFRAMEWORK_API UCoreTriggerAction : public ULogicObjectBase
{
public:
	GENERATED_BODY()

	/**
	* 动作执行
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnExecute", meta = (ScriptName = "OnExecute"))
	void K2_OnExecute(const FGameplayEventData& EventData) const;

	virtual void OnExecute(const FGameplayEventData& EventData) const;

	/**
	* 如果是技能动作，可以通过此方法获取技能
	*/
	UFUNCTION(BlueprintPure)
	class UCoreAbility* GetExecuteAbility(const FGameplayEventData& EventData) const;
};
