// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "PlayerHuntConditionProgress.generated.h"

/**
 * 玩家boss输出进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerHuntConditionProgress : public UCoreConditionProgress
{
public:
	GENERATED_BODY()

	/**
	* 输出贡献值
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	float ContributionDegree;

	virtual void PostProgressInitialize_Implementation() override;
	virtual TArray<TSubclassOf<class UGameEventBase>> GetCareEventTypes_Implementation() override;
	virtual bool ProgressGameEvent_Implementation(UGameEventBase* GameEvent) override;
	virtual bool IsComplete_Implementation() override;
	virtual void HandleComplete_Implementation() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
