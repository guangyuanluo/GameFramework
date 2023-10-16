// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "PlayerHuntCondition.generated.h"

/**
 * 玩家boss输出
 */
UCLASS(BlueprintType, meta = (DisplayName = "玩家boss输出", Category = "任务条件"))
class GAMEFRAMEWORK_API UPlayerHuntCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* boss id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FUnitIDContainer UnitIDContainer;

	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int MasterId;

	/**
	* 输出贡献值
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	float ContributionDegree;
};

/**
 * 玩家boss输出进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerHuntConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	/**
	* 输出贡献值
	*/
	UPROPERTY(Replicated, Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	float ContributionDegree;

	virtual bool IsComplete(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes() override;
	virtual void OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};