// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/Unit/UnitInfoConfigTableRow.h"

int FUnitInfoConfigTableRow::GetUniqueId() {
    return UnitId;
}

FString FUnitInfoConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { UnitId, UnitName });
}