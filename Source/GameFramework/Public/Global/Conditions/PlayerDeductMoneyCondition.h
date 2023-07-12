// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Money/MoneyTypeNumPair.h"
#include "PlayerDeductMoneyCondition.generated.h"

/**
 * 玩家扣除货币
 */
UCLASS(BlueprintType, meta = (DisplayName = "玩家扣除货币", Category = "任务条件"))
class GAMEFRAMEWORK_API UPlayerDeductMoneyCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 货币
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "扣除货币"))
	TArray<FMoneyTypeNumPair> Money;
};

/**
 * 玩家扣除金币条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerDeductMoneyConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	/**
	* 是否完成
	*/
	UPROPERTY(Replicated, Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	bool HaveComplete;

	/**
	* 订单id
	*/
	UPROPERTY(Replicated, Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	FGuid OrderID;

	virtual void OnInitialize_Implementation() override;
	virtual bool IsComplete_Implementation(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};