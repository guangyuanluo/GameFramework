// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CoreConditionObserver.generated.h"

class UCoreConditionProgress;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCoreConditionObserver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 条件观察者
 */
class GAMEFRAMEWORK_API ICoreConditionObserver
{
public:
	GENERATED_BODY()

	/**
	* 满足条件时回调
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ConditionSystem")
	void OnSatisfyConditions(const TArray<UCoreConditionProgress*>& Progresses);

	/**
	* 进度刷新
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ConditionSystem")
	void OnProgressRefresh(UCoreConditionProgress* ChangeProgress);

	/**
	* 不再关注
	*/
	void Unfollow();
};