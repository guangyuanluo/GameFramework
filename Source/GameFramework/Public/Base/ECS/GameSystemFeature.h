// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSystemFeature.generated.h"

/*
* @brief 系统特性，一个特性包含一组system
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API UGameSystemFeature : public UObject {
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<class USystemBase>> SystemClasses;
};