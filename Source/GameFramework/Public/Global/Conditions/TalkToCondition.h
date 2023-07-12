// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "TalkToCondition.generated.h"

/**
 * NPC谈话
 */
UCLASS(BlueprintType, meta = (DisplayName = "NPC谈话", Category = "任务条件"))
class GAMEFRAMEWORK_API UTalkToCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* @brief 与哪个npc对话
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FUnitIDContainer UnitIDContainer;
};

/**
 * 对话进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UTalkToConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	UPROPERTY(Replicated, Transient, BlueprintReadOnly, VisibleAnywhere)
	bool HaveTalk = false;

	virtual bool IsComplete_Implementation(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};