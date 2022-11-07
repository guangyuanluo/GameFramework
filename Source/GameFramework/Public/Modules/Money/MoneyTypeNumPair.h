// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoneyTypeNumPair.generated.h"

/**
* money类型数量对
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FMoneyTypeNumPair
{
	GENERATED_USTRUCT_BODY()

	/**
	*  货币类型ID
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Money", meta = (DisplayName = "货币类型ID"))
	int32 MoneyType;

	/**
	* 数量
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Money", meta = (DisplayName = "货币类型ID"))
	int32 Num;
};