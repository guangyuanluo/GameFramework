// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "PlayerQuestFinishConditionProgress.generated.h"

/**
 * 玩家完成任务条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerQuestFinishConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	/**
	* 是否完成
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool HaveComplete;

	virtual void PostProgressInitialize_Implementation() override;
	virtual TArray<TSubclassOf<class UGameEventBase>> GetCareEventTypes_Implementation() override;
	virtual bool ProgressGameEvent_Implementation(UGameEventBase* GameEvent) override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
