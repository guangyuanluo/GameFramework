// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExpInfo.generated.h"

/**
 * 经验信息
 */
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FExpInfo
{
	GENERATED_BODY()

	UPROPERTY(Category = "ExpInfo", EditAnywhere, BlueprintReadWrite)
	uint8 ExpType;

	UPROPERTY(Category = "ExpInfo", EditAnywhere, BlueprintReadWrite)
	int Level = 1;

	UPROPERTY(Category = "ExpInfo", EditAnywhere, BlueprintReadWrite)
	int Exp = 0;
};
