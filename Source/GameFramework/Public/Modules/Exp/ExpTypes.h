// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExpTypes.generated.h"

//经验类型，预留多个，可以在EnumDisplayNameConfigSetting设置DisplayName
UENUM(BlueprintType, meta = (DisplayName = "经验类型"))
enum class EExpTypeEnum :uint8 //设置uint8类型  
{
	ExpType_Role,
	ExpType_1,
	ExpType_2,
	ExpType_3,
	ExpType_4,
	ExpType_5,
	ExpType_6,
	ExpType_7,
	ExpType_8,
	ExpType_9,
	ExpType_10,
	ExpType_11,
	ExpType_12,
	ExpType_13,
	ExpType_14,
	ExpType_15,
	ExpType_16,
	ExpType_17,
	ExpType_18,
};