// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitGroupConfigTableRow.h"

int FUnitGroupConfigTableRow::GetUniqueId() {
    return GroupId;
}

FString FUnitGroupConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { GroupId, Description });
}

TArray<int>& FUnitGroupConfigTableRow::GetGroupItems() {
    return Units;
}