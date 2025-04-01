// Fill out your copyright notice in the Description page of Project Settings.


#include "BackpackComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

#include "AssetBackpack.h"
#include "CoreItem.h"
#include "CoreSceneItem.h"
#include "ItemComponent.h"
#include "GameEventUtils.h"
#include "GameEntity.h"
#include "BackpackSetting.h"
#include "BackpackTypeConfigTableRow.h"
#include "CoreCharacterStateBase.h"
#include "UE4LogImpl.h"
#include "ConfigTableCache.h"

// Sets default values for this component's properties
UBackpackComponent::UBackpackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UBackpackComponent::BeginPlay()
{
	Super::BeginPlay();

    if (Backpacks.Num() == 0) {
        const UBackpackSetting* BackpackSetting = GetDefault<UBackpackSetting>();
        if (BackpackSetting->InitPackageTypes.Num() > 0) {
            auto BackpackTypeDataTable = BackpackSetting->BackpackTypeTable.LoadSynchronous();

            if (BackpackTypeDataTable) {
                for (int Index = 0; Index < BackpackSetting->InitPackageTypes.Num(); ++Index) {
                    auto BackpackType = BackpackSetting->InitPackageTypes[Index];
                    auto BackpackTypeInfo = (FBackpackTypeConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(BackpackTypeDataTable, (int)BackpackType);
                    if (BackpackTypeInfo) {
                        FAssetBackpack NewAssetBackpack;
                        NewAssetBackpack.BackpackType = BackpackType;

                        for (int InitIndex = 0; InitIndex < BackpackTypeInfo->BackpackInitMaxNum; ++InitIndex) {
                            NewAssetBackpack.ItemList.Add(nullptr);
                        }

                        Backpacks.Add(NewAssetBackpack);
                    }
                }
            }
            else {
                UE_LOG(GameCore, Warning, TEXT("没有配置背包类型表"));
            }
        }
    }
}

void UBackpackComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    FDoRepLifetimeParams Params;

    Params.Condition = COND_OwnerOnly;
    DOREPLIFETIME_WITH_PARAMS_FAST(UBackpackComponent, Backpacks, Params);
}

bool UBackpackComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) {
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

    for (const auto& Backpack : Backpacks) {
		for (const auto& Item : Backpack.ItemList) {
			if (IsValid(Item)) {
				WroteSomething |= Channel->ReplicateSubobject(const_cast<UCoreItem*>(Item), *Bunch, *RepFlags);
			}
		}
    }

	return WroteSomething;
}

void UBackpackComponent::AddItem(EBackpackTypeEnum BackpackType, int32 ItemId, int32 Count, const FString& Reason, int32 SpecialSlot) {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (CharacterState) {
        auto GameEntity = Cast<IGameEntity>(CharacterState->GetPawn());
        if (GameEntity) {
            auto AddItemRequest = NewObject<UAddItemRequesEvent>();
            AddItemRequest->EntityId = GameEntity->GetEntityID();
            AddItemRequest->BackpackType = BackpackType;
            AddItemRequest->ItemId = ItemId;
            AddItemRequest->Count = Count;
            AddItemRequest->SpecialSlot = SpecialSlot;
            AddItemRequest->Reason = Reason;

            UGameEventUtils::PushEventToServer(this, AddItemRequest, false);
        }
    }
}

void UBackpackComponent::AddItems(const TArray<FAddItemInfo>& AddItems, const FString& Reason) {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (CharacterState) {
        auto GameEntity = Cast<IGameEntity>(CharacterState->GetPawn());
        if (GameEntity) {
            auto AddItemsRequest = NewObject<UAddItemsRequesEvent>();
            AddItemsRequest->EntityId = GameEntity->GetEntityID();
            AddItemsRequest->AddItemInfos = AddItems;
            AddItemsRequest->Reason = Reason;

            UGameEventUtils::PushEventToServer(this, AddItemsRequest, false);
        }
    }
}

void UBackpackComponent::UseItem(EBackpackTypeEnum BackpackType, const FString& InstanceID, int Count, const FString& Reason) {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (CharacterState) {
        auto GameEntity = Cast<IGameEntity>(CharacterState->GetPawn());
        if (GameEntity) {
            auto UseItemRequest = NewObject<UUseItemRequesEvent>();
            UseItemRequest->EntityId = GameEntity->GetEntityID();
            UseItemRequest->BackpackType = BackpackType;
            UseItemRequest->InstanceID = InstanceID;
            UseItemRequest->Count = Count;
            UseItemRequest->Reason = Reason;

            UGameEventUtils::PushEventToServer(this, UseItemRequest, false);
        }
    }
}

void UBackpackComponent::AbandonItem(EBackpackTypeEnum BackpackType, const FString& InstanceID, int Count, const FString& Reason) {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (CharacterState) {
        auto GameEntity = Cast<IGameEntity>(CharacterState->GetPawn());
        if (GameEntity) {
            auto AbandonItemRequest = NewObject<UAbandonItemRequesEvent>();
            AbandonItemRequest->EntityId = GameEntity->GetEntityID();
            AbandonItemRequest->BackpackType = BackpackType;
            AbandonItemRequest->InstanceID = InstanceID;
            AbandonItemRequest->Count = Count;
            AbandonItemRequest->Reason = Reason;

            UGameEventUtils::PushEventToServer(this, AbandonItemRequest, false);
        }
    }
}

void UBackpackComponent::DeductItem(EBackpackTypeEnum BackpackType, int32 ItemId, int Count, const FString& Reason, const FString& SpecialInstanceID) {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (CharacterState) {
        auto GameEntity = Cast<IGameEntity>(CharacterState->GetPawn());
        if (GameEntity) {
            auto DeductItemRequest = NewObject<UDeductItemRequesEvent>();
            DeductItemRequest->EntityId = GameEntity->GetEntityID();
            DeductItemRequest->BackpackType = BackpackType;
            DeductItemRequest->ItemId = ItemId;
            DeductItemRequest->Count = Count;
            DeductItemRequest->SpecialInstanceID = SpecialInstanceID;
            DeductItemRequest->Reason = Reason;

            UGameEventUtils::PushEventToServer(this, DeductItemRequest, false);
        }
    }
}

void UBackpackComponent::DeductItems(const TArray<FItemIDNumPair>& DeductItems, const FString& Reason) {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (CharacterState) {
        auto GameEntity = Cast<IGameEntity>(CharacterState->GetPawn());
        if (GameEntity) {
            auto DeductItemsRequest = NewObject<UDeductItemsRequesEvent>();
            DeductItemsRequest->EntityId = GameEntity->GetEntityID();
            DeductItemsRequest->DeductItems = DeductItems;
            DeductItemsRequest->Reason = Reason;

            UGameEventUtils::PushEventToServer(this, DeductItemsRequest, false);
        }
    }
}

void UBackpackComponent::MoveItem(EBackpackTypeEnum BackpackType, const FString& InstanceID, EBackpackTypeEnum NewPackageType, int NewSlotIndex) {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (CharacterState) {
        auto GameEntity = Cast<IGameEntity>(CharacterState->GetPawn());
        if (GameEntity) {
            auto MoveItemRequest = NewObject<UMoveItemRequesEvent>();
            MoveItemRequest->EntityId = GameEntity->GetEntityID();
            MoveItemRequest->BackpackType = BackpackType;
            MoveItemRequest->InstanceID = InstanceID;
            MoveItemRequest->NewPackageType = NewPackageType;
            MoveItemRequest->NewSlotIndex = NewSlotIndex;

            UGameEventUtils::PushEventToServer(this, MoveItemRequest, false);
        }
    }
}

void UBackpackComponent::SplitItem(EBackpackTypeEnum BackpackType, const FString& InstanceID, int Count) {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (CharacterState) {
        auto GameEntity = Cast<IGameEntity>(CharacterState->GetPawn());
        if (GameEntity) {
            auto SplitItemRequest = NewObject<USplitItemRequesEvent>();
            SplitItemRequest->EntityId = GameEntity->GetEntityID();
            SplitItemRequest->BackpackType = BackpackType;
            SplitItemRequest->InstanceID = InstanceID;
            SplitItemRequest->Count = Count;

            UGameEventUtils::PushEventToServer(this, SplitItemRequest, false);
        }
    }
}

void UBackpackComponent::PickupItem(EBackpackTypeEnum BackpackType, class ACoreSceneItem* DropItem, int PickupCount) {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (CharacterState) {
        auto GameEntity = Cast<IGameEntity>(CharacterState->GetPawn());
        if (GameEntity) {
            auto PickupItemRequest = NewObject<UPickupItemRequesEvent>();
            PickupItemRequest->EntityId = GameEntity->GetEntityID();
            PickupItemRequest->BackpackType = BackpackType;
            PickupItemRequest->DropEntityId = DropItem->GetEntityID();
            PickupItemRequest->Count = PickupCount;

            UGameEventUtils::PushEventToServer(this, PickupItemRequest, false);
        }
    }
}

void UBackpackComponent::SortBackpack(EBackpackTypeEnum BackpackType) {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (CharacterState) {
        auto GameEntity = Cast<IGameEntity>(CharacterState->GetPawn());
        if (GameEntity) {
            auto SortBackpackRequest = NewObject<USortBackpackRequestEvent>();
            SortBackpackRequest->EntityId = GameEntity->GetEntityID();
            SortBackpackRequest->BackpackType = BackpackType;

            UGameEventUtils::PushEventToServer(this, SortBackpackRequest, false);
        }
    }
}

void UBackpackComponent::NotifyBackpackChanged() {
	OnBackpackChanged();
}

int32 UBackpackComponent::GetBackpackItemCount(int32 ItemId) const {
	int TotalCount = 0;
	for (int BackpackIndex = 0; BackpackIndex < Backpacks.Num(); ++BackpackIndex) {
		for (int Index = 0; Index != Backpacks[BackpackIndex].ItemList.Num(); ++Index) {
			if (Backpacks[BackpackIndex].ItemList[Index]) {
				if (Backpacks[BackpackIndex].ItemList[Index]->ItemId == ItemId) {
					TotalCount += Backpacks[BackpackIndex].ItemList[Index]->ItemNum;
				}
			}
		}
	}
	return TotalCount;
}

FAssetBackpack InvalidAssetBackpack;

FAssetBackpack& UBackpackComponent::GetBackpack(EBackpackTypeEnum BackpackType) {
    for (int BackpackIndex = 0; BackpackIndex < Backpacks.Num(); ++BackpackIndex) {
        if (Backpacks[BackpackIndex].BackpackType == BackpackType) {
            return Backpacks[BackpackIndex];
        }
    }
	
    return InvalidAssetBackpack;
}

int UBackpackComponent::FindIndexByInstanceID(EBackpackTypeEnum BackpackType, const FString& InstanceID)
{
    for (int BackpackIndex = 0; BackpackIndex < Backpacks.Num(); ++BackpackIndex) {
        if (BackpackType == EBackpackTypeEnum::BackpackType_32 || Backpacks[BackpackIndex].BackpackType == BackpackType) {
            const auto& Backpack = Backpacks[BackpackIndex];
            for (auto ItemIndex = 0; ItemIndex < Backpack.ItemList.Num(); ++ItemIndex) {
                if (Backpack.ItemList[ItemIndex] && Backpack.ItemList[ItemIndex]->InstanceID == InstanceID) {
                    return ItemIndex;
                }
            }
        }
    }

    return -1;
}

void UBackpackComponent::OnBackpackChanged() {
	auto OnBackpackRefreshEvent = NewObject<UOnBackpackRefreshEvent>();
	OnBackpackRefreshEvent->BackpackComponent = this;
    UGameEventUtils::PushEvent(this, OnBackpackRefreshEvent);
}

FAssetBackpack& UBackpackComponent::FindOrAddPackage(EBackpackTypeEnum BackpackType) {
    const UBackpackSetting* BackpackSetting = GetDefault<UBackpackSetting>();
    auto BackpackTypeDataTable = BackpackSetting->BackpackTypeTable.LoadSynchronous();
    auto BackpackTypeInfo = (FBackpackTypeConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(BackpackTypeDataTable, (int)BackpackType);
    if (!BackpackTypeInfo) {
        return InvalidAssetBackpack;
    }

    auto& AssetBackpack = GetBackpack(BackpackType);
    if (!UAssetBackpackBlueprintLibrary::IsValid(AssetBackpack)) {
        FAssetBackpack NewAssetBackpack;
        NewAssetBackpack.BackpackType = BackpackType;

        for (int Index = 0; Index < BackpackTypeInfo->BackpackInitMaxNum; ++Index) {
            NewAssetBackpack.ItemList.Add(nullptr);
        }

        int NewPackageIndex = Backpacks.Add(NewAssetBackpack);
        return Backpacks[NewPackageIndex];
    }
    return AssetBackpack;
}