// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Exp/ExpTypes.h"
#include "PlayerReachExpLevelCondition.generated.h"

/**
 * 玩家达到经验等级条件
 */
UCLASS(BlueprintType, meta = (DisplayName = "玩家达到经验等级", Category = "通用条件"))
class GAMEFRAMEWORK_API UPlayerReachExpLevelCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 经验类型
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	EExpTypeEnum ExpType;

	/**
	* 经验等级
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 ExpLevel;
};

/**
 * 玩家达到经验等级条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerReachExpLevelConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	virtual bool IsComplete(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes() override;
	virtual void OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/
};