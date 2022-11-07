// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetBackpack.h"

const uint8 InvalidPackageType = -1;

FAssetBackpack::FAssetBackpack() {
    BackpackType = InvalidPackageType;
}

bool UAssetBackpackBlueprintLibrary::IsValid(UPARAM(ref)FAssetBackpack& AssetBackpack) {
    return AssetBackpack.BackpackType != InvalidPackageType;
}