// Fill out your copyright notice in the Description page of Project Settings.

#include "BackpackTypeConfigTableRow.h"

int FBackpackTypeConfigTableRow::GetUniqueId() {
    return (int)BackpackType;
}

int FBackpackTypeConfigTableRow::GetRowUniqueId() {
    return BackpackTypeId;
}

FString FBackpackTypeConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { (int)BackpackType, StaticEnum<EBackpackTypeEnum>()->GetDisplayNameTextByValue((int64)BackpackType).ToString() });
}