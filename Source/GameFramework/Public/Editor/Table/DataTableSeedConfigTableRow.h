// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataTableSeedConfigTableRow.generated.h"

/**
* @brief seed配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FDataTableSeedConfigTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DataTableSeedConfigTable")
	FString SeedKey;				///< seed的key

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DataTableSeedConfigTable")
	int32 SeedValue;				///< seed的value
};