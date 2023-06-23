// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoneyTypeContainer.generated.h"

/**
* 货币类型容器
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FMoneyTypeContainer
{
	GENERATED_BODY()

	/**
	*  货币类型
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "货币类型"))
	int32 MoneyType;
};