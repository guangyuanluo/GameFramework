// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Reward/CoreReward.h"
#include "Modules/Money/MoneyTypeNumPair.h"
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
	TArray<FMoneyTypeNumPair> Money;

	virtual FString GetNodeTitle_Implementation() override;
	virtual void HandleRewardDispatch_Implementation(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source) override;
};
