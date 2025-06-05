// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/Store/GoodsConfigTableRow.h"

int FGoodsConfigTableRow::GetUniqueId() {
    return GoodsID;
}

FString FGoodsConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { GoodsID, Description });
}