// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "SimpleGroupConfigTableRowBase.generated.h"

/**
* 简单组配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FSimpleGroupConfigTableRowBase : public FConfigTableRowBase
{
    GENERATED_USTRUCT_BODY()

    virtual TArray<int>& GetGroupItems();
};