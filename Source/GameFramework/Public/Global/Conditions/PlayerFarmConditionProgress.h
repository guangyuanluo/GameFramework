// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "PlayerFarmConditionProgress.generated.h"

/**
 * 砍怪条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerFarmConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	/**
	* 已经完成的数量
	*/
	UPROPERTY(Replicated, Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	int32 FinishCount;

	virtual void PostProgressInitialize_Implementation() override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
