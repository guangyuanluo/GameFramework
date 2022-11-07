// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemTypeConfigTableRow.h"

int FItemTypeConfigTableRow::GetUniqueId() {
    return ItemTypeId;
}

FString FItemTypeConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { ItemTypeId, ItemTypeDescription });
}