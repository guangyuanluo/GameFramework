// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/SimpleGroupConfigTableRowBase.h"
#include "StoreConfigTableRow.generated.h"

/**
* 商店配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FStoreConfigTableRow : public FSimpleGroupConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	*  商店id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StoreConfigTable", meta = (DisplayName = "商店Id", DisplayPriority = "1"))
	int32 StoreID;

	/**
	*  商店描述
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StoreConfigTable", meta = (DisplayName = "商店描述", DisplayPriority = "1"))
	FString Description;

	/**
	* 商店图标
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GoodsConfigTable", meta = (DisplayName = "商店图标", DisplayPriority = "1"))
	FString Icon;

	/**
	* 商店包含的商品
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StoreConfigTable|Hide", meta = (DisplayName = "商店商品", DisplayPriority = "1"))
	TArray<int> GoodsIDs;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
	virtual TArray<int>& GetGroupItems() override;
};