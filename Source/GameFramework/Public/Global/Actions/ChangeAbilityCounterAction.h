// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/TriggerAction/CoreTriggerAction.h"
#include "ChangeAbilityCounterAction.generated.h"

/**
 * 修改技能计数器
 */
UCLASS(meta=(DisplayName = "修改技能计数器", Category = "技能动作"))
class GAMEFRAMEWORK_API UChangeAbilityCounterAction : public UCoreTriggerAction
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
	* 差值
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DiffNum;

	virtual void OnExecute(const FGameplayEventData& EventData) const override;
};