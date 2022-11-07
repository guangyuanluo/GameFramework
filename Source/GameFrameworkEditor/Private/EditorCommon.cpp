// Fill out your copyright notice in the Description page of Project Settings.

#include "EditorCommon.h"

FConfigTableRowWrapper::~FConfigTableRowWrapper() {
    if (ConfigTableRow) {
        RowStruct->DestroyStruct(ConfigTableRow);
        FMemory::Free(ConfigTableRow);
    }
}