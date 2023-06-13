// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "PlayerCollectItemConditionProgress.generated.h"

/**
 * 玩家收集物品条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerCollectItemConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	virtual void PostProgressInitialize_Implementation() override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/
};
