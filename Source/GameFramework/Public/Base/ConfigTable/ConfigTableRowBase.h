// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ConfigTableRowBase.generated.h"

/**
* 配置基类表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FConfigTableRowBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
    /**
    * 获取唯一id
    */
    virtual int GetUniqueId();

    /**
    * 获取表唯一id
    */
    virtual int GetRowUniqueId();
    /**
    * 简单描述一行数据
    */
    virtual FString GetSimpleDescription();
    
    /**
    * 子类继承的自定义初始化，只会在运行时调用
    */
    virtual void CustomInit();
};