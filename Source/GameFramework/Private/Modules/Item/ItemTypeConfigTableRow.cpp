// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemTypeConfigTableRow.h"

int FItemTypeConfigTableRow::GetUniqueId() {
    return (int)ItemType;
}

int FItemTypeConfigTableRow::GetRowUniqueId() {
    return ItemTypeId;
}

FString FItemTypeConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { (int)ItemType, StaticEnum<EItemTypeEnum>()->GetDisplayNameTextByValue((int64)ItemType).ToString() });
}