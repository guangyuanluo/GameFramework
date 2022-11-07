// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "PlayerDeductItemConditionProgress.generated.h"

/**
 * 玩家扣除物品条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerDeductItemConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	/**
	* 是否完成
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	bool HaveComplete;

	/**
	* 订单id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	FGuid OrderID;

	virtual void PostProgressInitialize_Implementation() override;
	virtual TArray<TSubclassOf<class UGameEventBase>> GetCareEventTypes_Implementation() override;
	virtual bool ProgressGameEvent_Implementation(UGameEventBase* GameEvent) override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
