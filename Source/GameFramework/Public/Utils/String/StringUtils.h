// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StringUtils.generated.h"


/**
 * 字符串公共函数
 */
UCLASS()
class GAMEFRAMEWORK_API UStringUtils : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	/**
	* 字符串匹配开始
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool StartsWith(const FString& SearchStr, const FString& MatchStr, bool IgnoreCase = false);

	/**
	* 字符串匹配结束
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool EndsWith(const FString& SearchStr, const FString& MatchStr, bool IgnoreCase = false);

	/** 字符串转int64 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "String To Int64", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|String")
	static int64 Conv_StringToInt64(const FString& InString);

    static TArray<uint8> StringToBinary(const FString& Str);
    static FString BinaryToString(const TArray<uint8>& Data);
    static FString BinaryToString(uint8* Data, int Num);
};