// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/TriggerAction/CoreTriggerAction.h"
#include "SetAbilityCounterAction.generated.h"

/**
 * 设置技能计数器
 */
UCLASS(BlueprintType, meta=(DisplayName = "设置技能计数器", Category = "技能动作"))
class GAMEFRAMEWORK_API USetAbilityCounterAction : public UCoreTriggerAction
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

	/**
	* 计数器数量
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Num;

	virtual void OnExecute_Implementation(const FGameplayEventData& EventData) const override;
};