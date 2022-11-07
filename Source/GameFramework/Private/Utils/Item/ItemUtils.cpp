// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemUtils.h"
#include "ItemSetting.h"
#include "ItemConfigTableRow.h"
#include "ItemTypeConfigTableRow.h"
#include "ConfigTableCache.h"

uint8 UItemUtils::GetItemDefaultBackpackType(int ItemID) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
    if (ItemDataTable && ItemTypeDataTable) {
        auto ItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemID);
        if (ItemInfo) {
            auto ItemTypeInfo = (FItemTypeConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemTypeDataTable, ItemInfo->ItemType);
            if (ItemTypeInfo) {
                return ItemTypeInfo->DefaultBackpackType;
            }
        }
    }
    return -1;
}