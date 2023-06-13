// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "TalkToConditionProgress.generated.h"

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

	UFUNCTION(BlueprintCallable)
	bool IsQuestOtherProgressesComplete();

	virtual void PostProgressInitialize_Implementation() override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
