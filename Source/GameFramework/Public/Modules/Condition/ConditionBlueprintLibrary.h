// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ConditionBlueprintLibrary.generated.h"


/**
 * 条件公共函数
 */
UCLASS()
class GAMEFRAMEWORK_API UConditionBlueprintLibrary : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	/** 一组条件进度是否满足 */
	UFUNCTION(BlueprintPure)
	static bool DoesProgressesSatisfy(const TArray<class UCoreConditionProgress*>& Progresses);
};