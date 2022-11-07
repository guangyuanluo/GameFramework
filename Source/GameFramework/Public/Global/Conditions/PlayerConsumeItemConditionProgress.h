// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "PlayerConsumeItemConditionProgress.generated.h"

/**
 * 玩家消耗物品条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerConsumeItemConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	/**
	* 已经消耗的数量
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	int32 CurrentCount;

	virtual void PostProgressInitialize_Implementation() override;
	virtual TArray<TSubclassOf<class UGameEventBase>> GetCareEventTypes_Implementation() override;
	virtual bool ProgressGameEvent_Implementation(UGameEventBase* GameEvent) override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
