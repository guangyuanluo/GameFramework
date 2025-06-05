// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/Item/ItemConfigTableRow.h"

int FItemConfigTableRow::GetUniqueId() {
    return ItemId;
}

FString FItemConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { ItemId, ItemName });
}