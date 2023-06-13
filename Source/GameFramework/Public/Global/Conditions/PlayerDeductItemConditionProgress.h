// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "PlayerDeductItemConditionProgress.generated.h"

/**
 * 玩家扣除物品条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerDeductItemConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	/**
	* 是否完成
	*/
	UPROPERTY(Replicated, Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	bool HaveComplete;

	/**
	* 订单id
	*/
	UPROPERTY(Replicated, Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	FGuid OrderID;

	virtual void PostProgressInitialize_Implementation() override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
