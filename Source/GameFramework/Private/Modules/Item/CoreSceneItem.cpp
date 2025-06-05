// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/Item/CoreSceneItem.h"
#include "Modules/Item/ItemComponent.h"
#include "Base/ConfigTable/ConfigTableCache.h"
#include "Modules/Item/ItemSetting.h"
#include "Modules/Item/ItemConfigTableRow.h"
#include "Engine/StaticMesh.h"

ACoreSceneItem::ACoreSceneItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    SetRootComponent(StaticMeshComponent);
	ItemComponent = CreateDefaultSubobject<UItemComponent>("ItemComponent");

	bReplicates = true;
}

void ACoreSceneItem::UpdateItem(int ItemId, int ItemCount) {
    ItemComponent->IDNumPair.ItemIDContainer.ItemID = ItemId;
    bool HaveSet = false;
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();

    auto ItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemComponent->IDNumPair.ItemIDContainer.ItemID);
    if (ItemInfo) {
        auto StaticMesh = ItemInfo->ItemMesh.LoadSynchronous();
        if (StaticMesh) {
            HaveSet = true;

            StaticMeshComponent->SetStaticMesh(StaticMesh);
        }
    }
    if (!HaveSet) {
        StaticMeshComponent->SetStaticMesh(nullptr);
    }
}

void ACoreSceneItem::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

    UpdateItem(ItemComponent->IDNumPair.ItemIDContainer.ItemID, ItemComponent->IDNumPair.ItemNum);
}