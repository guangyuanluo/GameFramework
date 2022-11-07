// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "PlayerCollectItemConditionProgress.generated.h"

/**
 * 玩家收集物品条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerCollectItemConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	virtual void PostProgressInitialize_Implementation() override;
	virtual TArray<TSubclassOf<class UGameEventBase>> GetCareEventTypes_Implementation() override;
	virtual bool ProgressGameEvent_Implementation(UGameEventBase* GameEvent) override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;
};
