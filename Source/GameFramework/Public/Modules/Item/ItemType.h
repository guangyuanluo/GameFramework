// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class EBuiltInItemType : uint8 //设置uint8类型  
{
	E_Enquipment UMETA(DisplayName = "装备"),
	E_Weapon UMETA(DisplayName = "武器"),
	E_BuiltInMax = 10,
};