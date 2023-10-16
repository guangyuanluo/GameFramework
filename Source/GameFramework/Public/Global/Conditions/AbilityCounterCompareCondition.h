// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "Utils/Algorithm/ComparisonOperatorUtil.h"
#include "AbilityCounterCompareCondition.generated.h"

/**
 * 计数器比较
 */
UCLASS(meta = (DisplayName = "计数器比较", Category = "技能条件"))
class GAMEFRAMEWORK_API UAbilityCounterCompareCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 计数器名字
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FString CounterName;

	/**
	* 比较值
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int Value;

	/**
	* 比较运算符
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	ComparisonOperatorEnum Operator = ComparisonOperatorEnum::E_Equal;

	/**
	* 是否全局计数器，全局计数器不会随着条件销毁消失
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	bool IsGlobal = false;
};

UCLASS()
class GAMEFRAMEWORK_API UAbilityCounterCompareConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	virtual void OnInitialize() override;
	virtual void OnUninitialize() override;
	virtual bool IsComplete(bool& IsValid) override;

private:
	FString CounterName;

	UFUNCTION()
	void OnCounterChange(const FString& InCounterName, int InCounterNum);
};