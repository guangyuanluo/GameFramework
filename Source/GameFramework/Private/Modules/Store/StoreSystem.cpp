// Fill out your copyright notice in the Description page of Project Settings.

#include "StoreSystem.h"
#include "StoreGlobalEvents.h"
#include "ConfigTableCache.h"
#include "StoreSetting.h"
#include "GoodsConfigTableRow.h"
#include "StoreConfigTableRow.h"
#include "UE4LogImpl.h"
#include "CoreCharacter.h"
#include "CoreCharacterStateBase.h"
#include "WalletComponent.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "GameEntityManager.h"
#include "EventSystem.h"
#include "AssetSystem.h"
#include "BackpackTypeConfigTableRow.h"
#include "StoreComponent.h"
#include "GameFrameworkUtils.h"

void UStoreSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);

    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->RegistEventHandler(this);
}

void UStoreSystem::Uninitialize() {
	Super::Uninitialize();
}

bool UStoreSystem::BuyGoods(ACoreCharacter* Character, UStoreComponent* StoreComponent, int GoodsID) {
    if (!Character) {
        return false;
    }
    auto CharacterState = Cast<ACoreCharacterStateBase>(Character->GetPlayerState());
    if (!CharacterState) {
        UE_LOG(GameCore, Error, TEXT("错误对象，不存在CharacterState"));
        return false;
    }
    auto AssetSystem = GameInstance->GameSystemManager->GetSystemByClass<UAssetSystem>();
    if (!AssetSystem) {
        UE_LOG(GameCore, Error, TEXT("错误调用，不存在AssetSystem"));
        return false;
    }
    const UStoreSetting* StoreSetting = GetDefault<UStoreSetting>();
    auto StoreDataTable = StoreSetting->StoreTable.LoadSynchronous();
    auto GoodsDataTable = StoreSetting->GoodsTable.LoadSynchronous();
    if (StoreDataTable && GoodsDataTable) {
        int StoreID = StoreComponent->StoreID;

        auto StoreRow = (FStoreConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(StoreDataTable, StoreID);
        if (!StoreRow) {
            UE_LOG(GameCore, Warning, TEXT("商店ID不存在[%d]"), StoreID);
            return false;
        }
        if (!StoreRow->GoodsIDs.Contains(GoodsID)) {
            UE_LOG(GameCore, Warning, TEXT("商店ID不存在对应商品[%d] [%d]"), StoreID, GoodsID);
            return false;
        }
        auto GoodsRow = (FGoodsConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(GoodsDataTable, GoodsID);
        if (!GoodsRow) {
            UE_LOG(GameCore, Warning, TEXT("商品ID不存在[%d]"), GoodsID);
            return false;
        }
        TArray<FItemIDNumPair> CostItems;
        if (GoodsRow->CostItems.Num() > 0) {
            CostItems = GoodsRow->CostItems;

            if (!CharacterState->BackpackComponent) {
                UE_LOG(GameCore, Warning, TEXT("商品ID要求扣除物品，对象没有背包组件"));
                return false;
            }
            if (!AssetSystem->CanDeductItems(CharacterState->BackpackComponent, CostItems)) {
                UE_LOG(GameCore, Warning, TEXT("商品ID要求扣除物品，对象指定物品不足"));
                return false;
            }
        }
        TArray<FMoneyTypeNumPair> ChangeMoney;
        if (GoodsRow->CostMoney.Num() > 0) {
            ChangeMoney = GoodsRow->CostMoney;

            if (!CharacterState->WalletComponent) {
                UE_LOG(GameCore, Warning, TEXT("商品ID要求扣除货币，对象没有钱包组件"));
                return false;
            }
            if (!AssetSystem->CanDeductMoney(CharacterState->WalletComponent, ChangeMoney)) {
                UE_LOG(GameCore, Warning, TEXT("商品ID要求扣除货币，对象指定货币不足"));
                return false;
            }
        }
        TArray<FAddItemInfo> AddItems;
        if (GoodsRow->ContainItems.Num() > 0) {
            for (auto Item : GoodsRow->ContainItems) {
                FAddItemInfo AddItem;
                AddItem.BackpackType = FBackpackTypeConfigTableRow::BackpackTypeMax;
                AddItem.ItemId = Item.ItemID;
                AddItem.Count = Item.ItemNum;
                AddItems.Add(AddItem);
            }
            if (!CharacterState->BackpackComponent) {
                UE_LOG(GameCore, Warning, TEXT("商品ID要求添加物品，对象没有背包组件"));
                return false;
            }
            if (!AssetSystem->CanAddItems(CharacterState->BackpackComponent, AddItems)) {
                UE_LOG(GameCore, Warning, TEXT("商品ID要求添加物品，对象背包空间不足"));
                return false;
            }
        }
        TArray<FMoneyTypeNumPair> AddMoney;
        if (GoodsRow->ContainMoney.Num() > 0) {
            AddMoney = GoodsRow->ContainMoney;
        }
        TArray<FExpTypeNumPair> AddExps;
        if (GoodsRow->ContainExps.Num() > 0) {
            AddExps = GoodsRow->ContainExps;
        }

        FString Error;
        //扣物品货币
        if (CostItems.Num() > 0) {
            AssetSystem->DeductItems(CharacterState->BackpackComponent, CostItems, TEXT("BuyGoods"), Error);
        }
        if (ChangeMoney.Num() > 0) {
            for (auto& Money : ChangeMoney) {
                Money.Num = -Money.Num;
            }
            AssetSystem->ChangeMoney(CharacterState->WalletComponent, ChangeMoney, true, TEXT("BuyGoods"), Error);
        }
        //发放物品货币
        if (AddItems.Num() > 0) {
            AssetSystem->AddItems(CharacterState->BackpackComponent, AddItems, false, TEXT("BuyGoods"), Error);
        }
        if (AddMoney.Num() > 0) {
            AssetSystem->ChangeMoney(CharacterState->WalletComponent, AddMoney, false, TEXT("BuyGoods"), Error);
        }
        return true;
    }
    UE_LOG(GameCore, Error, TEXT("没有配置商店表和商品表"));
    return false;
}

TArray<UClass*> UStoreSystem::GetHandleEventTypes_Implementation() {
    return TArray<UClass*>({
        UBuyGoodsRequestEvent::StaticClass()
    }) ;
}

void UStoreSystem::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
    if (HandleEvent->IsA(UBuyGoodsRequestEvent::StaticClass())) {
        auto Request = Cast<UBuyGoodsRequestEvent>(HandleEvent);
        auto SourceEntity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityID);
        auto StoreEntity = InGameInstance->GameEntityManager->GetEntityById(Request->StoreEntityID);
        if (SourceEntity && StoreEntity) {
            auto SourceCharacter = Cast<ACoreCharacter>(SourceEntity.GetObject());
            auto Store = Cast<AActor>(StoreEntity.GetObject());
            if (SourceCharacter && Store) {
                auto StoreComponent = Cast<UStoreComponent>(Store->GetComponentByClass(UStoreComponent::StaticClass()));
                if (StoreComponent) {
                    BuyGoods(SourceCharacter, StoreComponent, Request->GoodsID);
                }
            }
        }        
    }
}