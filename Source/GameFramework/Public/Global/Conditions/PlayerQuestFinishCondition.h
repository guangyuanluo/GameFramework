// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "PlayerQuestFinishCondition.generated.h"

/**
 * 玩家任务完成条件
 */
UCLASS(BlueprintType, meta = (DisplayName = "任务完成", Category = "任务条件"))
class GAMEFRAMEWORK_API UPlayerQuestFinishCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 完成任务id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 FinishQuestId;
};

/**
 * 玩家完成任务条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerQuestFinishConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	/**
	* 是否完成
	*/
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	bool HaveComplete;

	virtual void OnInitialize_Implementation() override;
	virtual bool IsComplete_Implementation(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};