// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/TriggerAction/CoreTriggerAction.h"
#include "DestroyAbilityCounterAction.generated.h"

/**
 * 销毁技能计数器
 */
UCLASS(BlueprintType, meta=(DisplayName = "销毁技能计数器", Category = "技能动作"))
class GAMEFRAMEWORK_API UDestroyAbilityCounterAction : public UCoreTriggerAction
{
public:
	GENERATED_BODY()

	/**
	* 计数器名字
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CounterName;

	/**
	* 是否全局计数器
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsGlobal = false;

	virtual void OnExecute_Implementation(const FGameplayEventData& EventData) const override;
};