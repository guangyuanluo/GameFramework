// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "PlayerConsumeMoneyConditionProgress.generated.h"

/**
 * 玩家消耗金币条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerConsumeMoneyConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	/**
	* 已经消耗的数量
	*/
	UPROPERTY(Replicated, Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	int32 CurrentCount;

	virtual bool IsComplete_Implementation(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
