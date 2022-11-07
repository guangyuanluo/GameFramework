// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "MoneyTypeConfigTableRow.generated.h"

/**
* money类型配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FMoneyTypeConfigTableRow : public FConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	*  货币类型id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MoneyTypeConfigTable", meta = (DisplayName = "货币类型Id", DisplayPriority = "1"))
	int32 MoneyTypeId;

	/**
	*  货币类型描述
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MoneyTypeConfigTable", meta = (DisplayName = "货币类型描述", DisplayPriority = "1"))
	FString MoneyTypeDescription;

	/**
	* 货币icon
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MoneyTypeConfigTable", meta = (DisplayName = "货币图标", DisplayPriority = "1"))
	FString Icon;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
};