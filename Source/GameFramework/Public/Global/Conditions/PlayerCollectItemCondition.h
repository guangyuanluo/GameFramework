// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Item/ItemIDContainer.h"
#include "PlayerCollectItemCondition.generated.h"

/**
 * 收集物品条件
 */
UCLASS(BlueprintType, meta = (DisplayName = "收集物品", Category = "任务条件"))
class GAMEFRAMEWORK_API UPlayerCollectItemCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 物品id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FItemIDContainer ItemIDContainer;

	/**
	* 物品数量
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 ItemCount;
};

/**
 * 玩家收集物品条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerCollectItemConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	virtual void OnUninitialize() override;
	virtual bool IsComplete(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes() override;
	virtual void OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/
};
