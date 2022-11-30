// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectConfigTableRow.h"

int FEffectConfigTableRow::GetUniqueId() {
    return EffectID;
}

FString FEffectConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { EffectID, Description });
}