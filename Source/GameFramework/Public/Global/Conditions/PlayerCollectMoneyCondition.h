// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Money/MoneyTypeContainer.h"
#include "PlayerCollectMoneyCondition.generated.h"

/**
 * 玩家收集货币
 */
UCLASS(BlueprintType, meta = (DisplayName = "收集货币", Category = "任务条件"))
class GAMEFRAMEWORK_API UPlayerCollectMoneyCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 金币类型
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FMoneyTypeContainer MoneyTypeContainer;

	/**
	* 金币数量
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 MoneyCount;
};

/**
 * 玩家收集金币条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerCollectMoneyConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	virtual bool IsComplete_Implementation(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/
};