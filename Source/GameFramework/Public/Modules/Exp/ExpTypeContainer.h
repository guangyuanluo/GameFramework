// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExpTypeContainer.generated.h"

/**
* 经验类型容器
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FExpTypeContainer
{
	GENERATED_BODY()

	/**
	*  经验类型
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "经验类型"))
	int32 ExpType;
};