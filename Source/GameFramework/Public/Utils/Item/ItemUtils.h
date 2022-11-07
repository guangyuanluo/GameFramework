// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemUtils.generated.h"


/**
 * 
 */
UCLASS()
class GAMEFRAMEWORK_API UItemUtils : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Utils")
	static uint8 GetItemDefaultBackpackType(int ItemID);
};