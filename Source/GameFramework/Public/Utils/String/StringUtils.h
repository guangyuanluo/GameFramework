// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StringUtils.generated.h"


/**
 * �ַ�����������
 */
UCLASS()
class GAMEFRAMEWORK_API UStringUtils : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	/**
	* �ַ���ƥ�俪ʼ
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool StartsWith(const FString& SearchStr, const FString& MatchStr, bool IgnoreCase = false);

	/**
	* �ַ���ƥ�����
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool EndsWith(const FString& SearchStr, const FString& MatchStr, bool IgnoreCase = false);
};