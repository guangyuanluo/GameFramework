// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Reward/CoreReward.h"
#include "IntimacyReward.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UIntimacyReward : public UCoreReward
{
public:
	GENERATED_UCLASS_BODY()

	UPROPERTY(Category = "Reward", EditAnywhere, BlueprintReadOnly)
	int UnitId;

	UPROPERTY(Category = "Reward", EditAnywhere, BlueprintReadOnly)
	int32 IntimacyValue;

	virtual void HandleRewardDispatch(class UCoreGameInstance* gameInstance, TScriptInterface<class IGameEntity> target) override;
};
