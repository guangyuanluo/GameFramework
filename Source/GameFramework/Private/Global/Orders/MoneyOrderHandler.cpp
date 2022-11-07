// Fill out your copyright notice in the Description page of Project Settings.

#include "MoneyOrderHandler.h"
#include "MoneyOrder.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "AssetSystem.h"
#include "CoreCharacter.h"
#include "CoreCharacterStateBase.h"

class UCoreOrderBase* UMoneyOrderHandler::OnCreateOrder_Implementation(UCoreGameInstance* InGameInstance, TSubclassOf<class UCoreOrderBase> OrderClass, UObject* Context) {
    auto MoneyOrder = NewObject<UMoneyOrder>(InGameInstance, OrderClass);
    return MoneyOrder;
}

bool UMoneyOrderHandler::OnPayOrder_Implementation(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source, class UCoreOrderBase* Order) {
    auto MoneyOrder = Cast<UMoneyOrder>(Order);
    if (!MoneyOrder) {
        return false;
    }
    if (MoneyOrder->Money.Num() == 0) {
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
    if (!CoreCharacterStateBase->WalletComponent) {
        return false;
    }

    TArray<FMoneyTypeNumPair> Money;
    for (auto MoneyItem : MoneyOrder->Money) {
        Money.Add(MoneyItem);
    }

    if (!AssetSystem->CanDeductMoney(CoreCharacterStateBase->WalletComponent, Money)) {
        return false;
    }

    for (int Index = 0; Index < Money.Num(); ++Index) {
        Money[Index].Num = -Money[Index].Num;
    }

    FString Error;
    AssetSystem->ChangeMoney(CoreCharacterStateBase->WalletComponent, Money, true, TEXT("Order"), Error);

    return true;
}