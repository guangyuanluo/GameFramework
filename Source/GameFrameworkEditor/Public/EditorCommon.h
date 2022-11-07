// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorCommon.generated.h"

USTRUCT(BlueprintType)
struct GAMEFRAMEWORKEDITOR_API FConfigTableRowWrapper {
    GENERATED_USTRUCT_BODY()

    uint8* ConfigTableRow;

    /** Structure to use for each Row of the table, must inherit from FTableRowBase */
    UPROPERTY(VisibleAnywhere, Category = DataTable, meta = (DisplayThumbnail = "false"))
    const UScriptStruct* RowStruct;

    virtual ~FConfigTableRowWrapper();
};