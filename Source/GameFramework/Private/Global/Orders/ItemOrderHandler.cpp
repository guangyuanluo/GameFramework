// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemOrderHandler.h"
#include "ItemOrder.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "AssetSystem.h"
#include "CoreCharacter.h"
#include "CoreCharacterStateBase.h"

class UCoreOrderBase* UItemOrderHandler::OnCreateOrder_Implementation(UCoreGameInstance* InGameInstance, TSubclassOf<class UCoreOrderBase> OrderClass, UObject* Context) {
    auto ItemOrder = NewObject<UItemOrder>(InGameInstance, OrderClass);
    return ItemOrder;
}

bool UItemOrderHandler::OnPayOrder_Implementation(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source, class UCoreOrderBase* Order) {
    auto ItemOrder = Cast<UItemOrder>(Order);
    if (!ItemOrder) {
        return false;
    }
    if (ItemOrder->Items.Num() == 0) {
        return true;
    }
    auto AssetSystem = InGameInstance->GameSystemManager->GetSystemByClass<UAssetSystem>();
    if (!AssetSystem) {
        return false;
    }
    if (!Source) {
        return false;
    }
    auto CoreCharacterStateBase = Cast<ACoreCharacterStateBase>(Source->GetPlayerState());
    if (!CoreCharacterStateBase) {
        return false;
    }
    if (!CoreCharacterStateBase->BackpackComponent) {
        return false;
    }
    
    FString Error;
    if (!AssetSystem->DeductItems(CoreCharacterStateBase->BackpackComponent, ItemOrder->Items, TEXT("Order"), Error)) {
        return false;
    }

    return true;
}