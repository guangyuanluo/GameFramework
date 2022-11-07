// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RewardType.h"
#include "CoreReward.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UCoreReward : public UObject
{
public:
	GENERATED_BODY()

	UPROPERTY(transient, Category = "Reward", EditAnywhere, BlueprintReadOnly)
	RewardType RewardType;

	virtual void HandleRewardDispatch(class UCoreGameInstance* gameInstance, TScriptInterface<class IGameEntity> target) { }
};
