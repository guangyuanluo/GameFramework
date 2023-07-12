// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionProgressLifeCycleManager.generated.h"

/*
* 条件进度生命周期管理
*/
UCLASS()
class GAMEFRAMEWORK_API UConditionProgressLifeCycleManager : public UObject {
	GENERATED_BODY()

public:
	/**
	* 请求一个Progress
	*/
	virtual class UCoreConditionProgress* MallocProgress(AActor* ProgressOwner, class UCoreCondition* Condition);
	/**
	* 释放一个progress
	*/
	virtual void FreeProgress(class UCoreConditionProgress* Progress);
};