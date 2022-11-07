// Fill out your copyright notice in the Description page of Project Settings.

#include "ExpTypeConfigTableRow.h"

int FExpTypeConfigTableRow::GetUniqueId() {
    return ExpTypeId;
}

FString FExpTypeConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { ExpTypeId, ExpTypeDescription });
}