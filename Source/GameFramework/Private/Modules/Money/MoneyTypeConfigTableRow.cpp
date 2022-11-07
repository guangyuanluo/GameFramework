// Fill out your copyright notice in the Description page of Project Settings.

#include "MoneyTypeConfigTableRow.h"

int FMoneyTypeConfigTableRow::GetUniqueId() {
    return MoneyTypeId;
}

FString FMoneyTypeConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { MoneyTypeId, MoneyTypeDescription });
}