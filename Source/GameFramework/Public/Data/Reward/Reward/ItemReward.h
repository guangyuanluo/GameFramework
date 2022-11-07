// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Reward/CoreReward.h"
#include "ItemReward.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UItemReward : public UCoreReward
{
public:
	GENERATED_UCLASS_BODY()

	UPROPERTY(Category = "Reward", EditAnywhere, BlueprintReadOnly)
	int32 ItemId;

	UPROPERTY(Category = "Reward", EditAnywhere, BlueprintReadOnly)
	int32 BackpackType;

	UPROPERTY(Category = "Reward", EditAnywhere, BlueprintReadOnly)
	int32 Count;

	virtual void HandleRewardDispatch(class UCoreGameInstance* GameInstance, TScriptInterface<class IGameEntity> Target) override;
};
