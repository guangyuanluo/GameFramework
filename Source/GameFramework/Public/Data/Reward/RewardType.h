// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//需要标记枚举，并设置蓝图类型  
UENUM(BlueprintType)
enum class RewardType :uint8 //设置uint8类型  
{
	E_Item UMETA(DisplayName = "物品"),
	E_Money UMETA(DisplayName = "货币"),
	E_Exp UMETA(DisplayName = "经验"),
	E_Intimacy UMETA(DisplayName = "好感度"),
};