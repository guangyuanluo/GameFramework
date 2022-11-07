// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Reward/CoreReward.h"
#include "ExpReward.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UExpReward : public UCoreReward
{
public:
	GENERATED_UCLASS_BODY()

	UPROPERTY(Category = "Reward", EditAnywhere, BlueprintReadOnly)
	uint8 ExpType;

	UPROPERTY(Category = "Reward", EditAnywhere, BlueprintReadOnly)
	int Count;

	virtual void HandleRewardDispatch(class UCoreGameInstance* gameInstance, TScriptInterface<class IGameEntity> target) override;
};
