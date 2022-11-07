// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

#include "CoreGameInstance.h"
#include "StoreSetting.h"
#include "StoreConfigTableRow.h"
#include "ConfigTableCache.h"
#include "StoreGlobalEvents.h"
#include "GameSystemManager.h"
#include "EventSystem.h"

// Sets default values for this component's properties
UStoreComponent::UStoreComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UStoreComponent::BeginPlay()
{
	Super::BeginPlay();

    const UStoreSetting* StoreSetting = GetDefault<UStoreSetting>();
    auto StoreDataTable = StoreSetting->StoreTable.LoadSynchronous();
    if (StoreDataTable) {
        auto FindStoreRow = (FStoreConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(StoreDataTable, StoreID);
        if (FindStoreRow) {
            if (FindStoreRow->GoodsIDs.Num() > 0) {
                for (auto GoodsID : FindStoreRow->GoodsIDs) {
                    FStoreGoodsInfo GoodsInfo;
                    GoodsInfo.GoodsID = GoodsID;
                    GoodsInfos.Add(GoodsInfo);
                }
            }
        }
    }
}

void UStoreComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStoreComponent, GoodsInfos);
}

void UStoreComponent::BuyGoods(ACoreCharacter* Character, int32 GoodsID) {
    int FindGoodsIndex = -1;
    for (int Index = 0; Index < GoodsInfos.Num(); ++Index) {
        if (GoodsInfos[Index].GoodsID == GoodsID) {
            FindGoodsIndex = Index;
            break;
        }
    }
    if (FindGoodsIndex == -1) {
        return;
    }

	auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();

    auto BuyGoodsRequestEvent = NewObject<UBuyGoodsRequestEvent>();
    BuyGoodsRequestEvent->Source = Character;
    BuyGoodsRequestEvent->Store = GetOwner();
    BuyGoodsRequestEvent->GoodsID = GoodsID;

    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEventToServer(BuyGoodsRequestEvent, false);
}

void UStoreComponent::OnGoodsChanged() {

}