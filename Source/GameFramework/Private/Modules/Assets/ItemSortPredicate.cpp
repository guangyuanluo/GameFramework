// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/Assets/ItemSortPredicate.h"

bool UItemSortPredicate::Compare_Implementation(int itemIdA, int itemIdB) {
    return itemIdA > itemIdB;
}