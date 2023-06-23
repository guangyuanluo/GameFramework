// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemReward.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "AssetSystem.h"
#include "CoreCharacter.h"
#include "CoreCharacterStateBase.h"

UItemReward::UItemReward(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

FString UItemReward::GetNodeTitle_Implementation() {
    return TEXT("物品奖励");
}

void UItemReward::HandleRewardDispatch_Implementation(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source) {
    if (!Source) {
        return;
    }
    auto CharacterState = Cast<ACoreCharacterStateBase>(Source->GetPlayerState());
    if (!CharacterState) {
        return;
    }
    if (!CharacterState->BackpackComponent) {
        return;
    }
    if (Items.Num() == 0) {
        return;
    }
    auto AssetSystem = InGameInstance->GameSystemManager->GetSystemByClass<UAssetSystem>();
    if (!AssetSystem) {
        return;
    }
    TArray<FAddItemInfo> AddItems;
    for (int Index = 0; Index < Items.Num(); ++Index) {
        auto Item = Items[Index];
        FAddItemInfo AddItemInfo;
        AddItemInfo.ItemId = Item.ItemIDContainer.ItemID;
        AddItemInfo.Count = Item.ItemNum;
        AddItems.Add(MoveTemp(AddItemInfo));
    }
    FString Error;
    AssetSystem->AddItems(CharacterState->BackpackComponent, AddItems, true, TEXT("Reward"), Error);
}