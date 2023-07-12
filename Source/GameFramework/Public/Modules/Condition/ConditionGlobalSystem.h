// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/Subsystem/GameWorldSubsystem.h"
#include "ConditionGlobalSystem.generated.h"

/*
* 条件全局系统
*/
UCLASS(config = Game)
class GAMEFRAMEWORK_API UConditionGlobalSystem : public UGameWorldSubsystem {
	GENERATED_BODY()

public:
	/**
	* 请求一个Progress
	*/
	class UCoreConditionProgress* MallocProgress(AActor* ProgressOwner, class UCoreCondition* Condition);
	
	/**
	* 释放一个progress
	*/
	void FreeProgress(class UCoreConditionProgress* Progress);

private:
	UPROPERTY(config)
	TSubclassOf<class UConditionProgressLifeCycleManager> ProgressLifeCycleManager;

	UPROPERTY()
	class UConditionProgressLifeCycleManager* ProgressLifeCycleManagerCDO;
};