// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitBlueprintLibrary.h"

bool UUnitBlueprintLibrary::IsUnitIDContainersContainerID(const TArray<FUnitIDContainer>& UnitIDContainers, int32 UnitID) {
    for (const auto& UnitIDContainer : UnitIDContainers) {
        if (UnitIDContainer.UnitID == UnitID) {
            return true;
        }
    }
    return false;
}