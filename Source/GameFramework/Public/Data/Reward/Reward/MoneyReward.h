// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Reward/CoreReward.h"
#include "MoneyReward.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UMoneyReward : public UCoreReward
{
public:
	GENERATED_UCLASS_BODY()

	UPROPERTY(Category = "Reward", EditAnywhere, BlueprintReadOnly)
	uint8 MoneyType;

	UPROPERTY(Category = "Reward", EditAnywhere, BlueprintReadOnly)
	int MoneyCount;

	virtual void HandleRewardDispatch(class UCoreGameInstance* GameInstance, TScriptInterface<class IGameEntity> Target) override;
};
