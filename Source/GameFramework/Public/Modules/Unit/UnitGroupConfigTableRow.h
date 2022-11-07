// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/SimpleGroupConfigTableRowBase.h"
#include "UnitGroupConfigTableRow.generated.h"

/**
* 单位组配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FUnitGroupConfigTableRow : public FSimpleGroupConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	*  单位组Id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnitGroupConfigTableRow", meta = (DisplayName = "单位组Id"))
	int GroupId;

	/**
	*  单位组描述
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnitGroupConfigTableRow", meta = (DisplayName = "单位组描述"))
	FString Description;

	/**
	*  包含单位
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnitGroupConfigTableRow|Hide", meta = (DisplayName = "包含单位"))
	TArray<int> Units;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
    virtual TArray<int>& GetGroupItems() override;
};