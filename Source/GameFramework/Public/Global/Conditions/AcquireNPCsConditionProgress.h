// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "AcquireNPCsConditionProgress.generated.h"

/**
 * 占有NPC进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UAcquireNPCsConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	/**
	* 是否占住NPC
	*/
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	bool HaveAcquire = false;

	virtual void OnInitialize_Implementation() override;
	virtual void OnUninitialize_Implementation() override;
	virtual bool IsComplete_Implementation(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};