// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StringUtils.generated.h"


/**
 * ×Ö·û´®¹«¹²º¯Êý
 */
UCLASS()
class GAMEFRAMEWORK_API UStringUtils : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	/**
	* ×Ö·û´®Æ¥Åä¿ªÊ¼
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool StartsWith(const FString& SearchStr, const FString& MatchStr, bool IgnoreCase = false);

	/**
	* ×Ö·û´®Æ¥Åä½áÊø
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool EndsWith(const FString& SearchStr, const FString& MatchStr, bool IgnoreCase = false);
};