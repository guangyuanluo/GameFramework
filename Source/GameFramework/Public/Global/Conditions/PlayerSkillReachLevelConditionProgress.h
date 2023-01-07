// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "PlayerSkillReachLevelConditionProgress.generated.h"

/**
 * 玩家技能达到等级进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerSkillReachLevelConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	virtual void PostProgressInitialize_Implementation() override;
	virtual TArray<TSubclassOf<class UGameEventBase>> GetCareEventTypes_Implementation() override;
	virtual bool ProgressGameEvent_Implementation(UGameEventBase* GameEvent) override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;
};
