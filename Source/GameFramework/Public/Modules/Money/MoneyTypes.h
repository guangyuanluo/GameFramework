// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoneyTypes.generated.h"

//任务类型
UENUM(BlueprintType, meta = (DisplayName = "金币类型"))
enum class EMoneyTypeEnum :uint8 //设置uint8类型  
{
	MoneyType_1,
	MoneyType_2,
	MoneyType_3,
	MoneyType_4,
	MoneyType_5,
	MoneyType_6,
	MoneyType_7,
	MoneyType_8,
	MoneyType_9,
	MoneyType_10,
	MoneyType_11,
	MoneyType_12,
	MoneyType_13,
	MoneyType_14,
	MoneyType_15,
	MoneyType_16,
	MoneyType_17,
	MoneyType_18,
	MoneyType_19,
};