// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "Modules/Exp/ExpTypes.h"
#include "UnitReachExpLevelCondition.generated.h"

/**
 * 单位达到经验等级条件
 */
UCLASS(BlueprintType, meta = (DisplayName = "单位达到经验等级", Category = "通用条件"))
class GAMEFRAMEWORK_API UUnitReachExpLevelCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 单位id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FUnitIDContainer UnitIDContainer;

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
 * 单位达到经验等级条件进度

 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UUnitReachExpLevelConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	virtual bool IsComplete(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes() override;
	virtual void OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/
};