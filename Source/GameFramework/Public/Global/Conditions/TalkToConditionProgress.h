// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "TalkToConditionProgress.generated.h"

/**
 * 对话进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UTalkToConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	UPROPERTY(Transient, BlueprintReadOnly, VisibleAnywhere)
	bool HaveTalk = false;

	UFUNCTION(BlueprintCallable)
	bool IsQuestOtherProgressesComplete();

	virtual void PostProgressInitialize_Implementation() override;
	virtual TArray<TSubclassOf<class UGameEventBase>> GetCareEventTypes_Implementation() override;
	virtual bool ProgressGameEvent_Implementation(UGameEventBase* GameEvent) override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};