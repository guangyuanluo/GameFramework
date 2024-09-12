// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BackpackTypes.generated.h"

//背包类型，预留多个，可以在EnumDisplayNameConfigSetting设置DisplayName
UENUM(BlueprintType, meta = (DisplayName = "背包类型"))
enum class EBackpackTypeEnum :uint8 //设置uint8类型  
{
	BackpackType_1,
	BackpackType_2,
	BackpackType_3,
	BackpackType_4,
	BackpackType_5,
	BackpackType_6,
	BackpackType_7,
	BackpackType_8,
	BackpackType_9,
	BackpackType_10,
	BackpackType_11,
	BackpackType_12,
	BackpackType_13,
	BackpackType_14,
	BackpackType_15,
	BackpackType_16,
	BackpackType_17,
	BackpackType_18,
	BackpackType_19,
	BackpackType_20,
	BackpackType_21,
	BackpackType_22,
	BackpackType_23,
	BackpackType_24,
	BackpackType_25,
	BackpackType_26,
	BackpackType_27,
	BackpackType_28,
	BackpackType_29,
	BackpackType_30,
	BackpackType_31,
	BackpackType_32,
};