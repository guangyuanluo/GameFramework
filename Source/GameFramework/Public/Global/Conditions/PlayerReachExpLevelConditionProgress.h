// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "PlayerReachExpLevelConditionProgress.generated.h"

/**
 * 玩家达到经验等级条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerReachExpLevelConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	virtual bool IsComplete_Implementation() override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/
};
