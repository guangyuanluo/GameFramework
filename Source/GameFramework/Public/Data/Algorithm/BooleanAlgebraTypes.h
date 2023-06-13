// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BooleanAlgebraTypes.generated.h"

/**
 * 布尔运算关系符
 */
UENUM(BlueprintType)
enum class BooleanAlgebraEnum :uint8 {
    E_AND UMETA(DisplayName = "与"),
    E_OR UMETA(DisplayName = "或"),
    E_XOR UMETA(DisplayName = "异或"),
};