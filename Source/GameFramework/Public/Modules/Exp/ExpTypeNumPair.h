// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExpTypeNumPair.generated.h"

/**
* 经验类型数量对
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FExpTypeNumPair
{
	GENERATED_USTRUCT_BODY()

	/**
	* 经验类型ID
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Exp", meta = (DisplayName = "经验类型ID"))
	int32 ExpType;

	/**
	* 经验数量
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Exp", meta = (DisplayName = "经验数量"))
	int32 ExpNum;
};