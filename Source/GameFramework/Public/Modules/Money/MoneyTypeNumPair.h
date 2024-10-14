// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Money/MoneyTypes.h"
#include "MoneyTypeNumPair.generated.h"

/**
* money类型数量对
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FMoneyTypeNumPair
{
	GENERATED_USTRUCT_BODY()

	/**
	*  货币类型
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Money")
	EMoneyTypeEnum MoneyType;

	/**
	* 数量
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Money")
	int32 Num;
};