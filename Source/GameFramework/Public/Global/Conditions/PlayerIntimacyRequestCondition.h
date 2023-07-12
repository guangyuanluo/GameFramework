// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "PlayerIntimacyRequestCondition.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EIntimacyCompare : uint8
{
	Above 	UMETA(DisplayName = "高于"),
	Below 	UMETA(DisplayName = "低于"),
};
/**
 * 玩家好感度需求条件
 */
UCLASS(BlueprintType, meta = (DisplayName = "玩家好感度需求", Category = "任务条件"))
class GAMEFRAMEWORK_API UPlayerIntimacyRequestCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* npcid
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FUnitIDContainer NPCIDContainer;

	/**
	* 好感度需求
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 IntimacyRequest;

	/**
	* 好感度需求方式
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	EIntimacyCompare Compare;
};

/**
 * 玩家好感度需求条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerIntimacyRequestConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	virtual bool IsComplete_Implementation(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/
};