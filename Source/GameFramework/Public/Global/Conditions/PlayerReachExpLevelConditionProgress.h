// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "PlayerReachExpLevelConditionProgress.generated.h"

/**
 * 玩家达到经验等级条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerReachExpLevelConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	virtual void PostProgressInitialize_Implementation() override;
	virtual TArray<TSubclassOf<class UGameEventBase>> GetCareEventTypes_Implementation() override;
	virtual bool ProgressGameEvent_Implementation(UGameEventBase* GameEvent) override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;
};
