// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "Modules/Item/ItemIDNumPair.h"
#include "Modules/Money/MoneyTypeNumPair.h"
#include "Modules/Exp/ExpTypeNumPair.h"
#include "GoodsConfigTableRow.generated.h"

/**
* 商品配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FGoodsConfigTableRow : public FConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	*  商品id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GoodsConfigTable", meta = (DisplayName = "商品Id", DisplayPriority = "1"))
	int32 GoodsID;

	/**
	*  商品描述
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GoodsConfigTable", meta = (DisplayName = "商品描述", DisplayPriority = "1"))
	FString Description;

	/**
	* 商品图标
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GoodsConfigTable", meta = (DisplayName = "商品图标", DisplayPriority = "1"))
	FString Icon;

	/**
	*  商品包含物品
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GoodsConfigTable", meta = (DisplayName = "包含物品", DisplayPriority = "1"))
	TArray<FItemIDNumPair> ContainItems;

	/**
	*  商品包含货币
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GoodsConfigTable", meta = (DisplayName = "包含货币", DisplayPriority = "1"))
	TArray<FMoneyTypeNumPair> ContainMoney;

	/**
	*  商品包含经验
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GoodsConfigTable", meta = (DisplayName = "包含经验", DisplayPriority = "1"))
	TArray<FExpTypeNumPair> ContainExps;

	/**
	*  商品消耗物品，以物易物
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GoodsConfigTable", meta = (DisplayName = "消耗物品", DisplayPriority = "1"))
	TArray<FItemIDNumPair> CostItems;

	/**
	*  商品消耗货币，以钱易物
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GoodsConfigTable", meta = (DisplayName = "消耗货币", DisplayPriority = "1"))
	TArray<FMoneyTypeNumPair> CostMoney;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
};