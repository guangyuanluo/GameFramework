// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Money/MoneyTypeContainer.h"
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
	FMoneyTypeContainer MoneyTypeContainer;

	/**
	* 数量
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Money", meta = (DisplayName = "货币类型ID"))
	int32 Num;
};