// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreReward.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UCoreReward : public UObject
{
public:
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintNativeEvent)
	FString GetNodeTitle();

    /**
    * ��������
    */
    UFUNCTION(BlueprintNativeEvent)
    void HandleRewardDispatch(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source);
};