// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetBackpack.h"

FAssetBackpack::FAssetBackpack() {
    BackpackType = EBackpackTypeEnum::BackpackType_32;
}

bool UAssetBackpackBlueprintLibrary::IsValid(UPARAM(ref)FAssetBackpack& AssetBackpack) {
    return AssetBackpack.BackpackType != EBackpackTypeEnum::BackpackType_32;
}