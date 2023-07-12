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
	UFUNCTION(BlueprintNativeEvent)
	void OnExecute(const FGameplayEventData& EventData) const;
};
