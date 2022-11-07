// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "BackpackTypeConfigTableRow.generated.h"

/**
* package配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FBackpackTypeConfigTableRow : public FConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	static int BackpackTypeMax;

	/**
	*  背包类型id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PackageTypeConfigTable", meta = (DisplayName = "背包类型Id", DisplayPriority = "1"))
	int32 BackpackTypeId;

	/**
	*  背包初始上限
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PackageTypeConfigTable", meta = (DisplayName = "背包初始上限", DisplayPriority = "1"))
	int32 BackpackInitMaxNum = 1;

	/**
	*  背包扩充最大上限
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PackageTypeConfigTable", meta = (DisplayName = "背包扩充最大上限", DisplayPriority = "1"))
	int32 BackpackExpansionMaxNum = 1;

	/**
	*  背包类型描述
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PackageTypeConfigTable", meta = (DisplayName = "背包类型描述", DisplayPriority = "1"))
	FString BackpackTypeDescription;

	/**
	*  允许留空
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PackageTypeConfigTable", meta = (DisplayName = "是否允许留空", DisplayPriority = "1"))
	bool AllowBlank = true;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
};