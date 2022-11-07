// Fill out your copyright notice in the Description page of Project Settings.

#include "StoreConfigTableRow.h"

int FStoreConfigTableRow::GetUniqueId() {
    return StoreID;
}

FString FStoreConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { StoreID, Description });
}

TArray<int>& FStoreConfigTableRow::GetGroupItems() {
    return GoodsIDs;
}