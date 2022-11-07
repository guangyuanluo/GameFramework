// Fill out your copyright notice in the Description page of Project Settings.

#include "BackpackTypeConfigTableRow.h"

int FBackpackTypeConfigTableRow::BackpackTypeMax = 200;

int FBackpackTypeConfigTableRow::GetUniqueId() {
    return BackpackTypeId;
}

FString FBackpackTypeConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { BackpackTypeId, BackpackTypeDescription });
}