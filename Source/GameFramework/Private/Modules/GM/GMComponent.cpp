// Fill out your copyright notice in the Description page of Project Settings.

#include "GMComponent.h"
#include "Engine/World.h"
#include "CoreGameInstance.h"
#include "UE4LogImpl.h"
#include "GameEntityManager.h"
#include "GameSystemManager.h"
#include "CoreSceneItem.h"
#include "CoreCharacterStateBase.h"
#include "PlayerComponent.h"
#include "BackpackComponent.h"
#include "WalletComponent.h"
#include "ExpComponent.h"
#include "CorePlayerController.h"
#include "CoreItem.h"

void UGMComponent::AddItemInEditor(int32 BackpackType, int32 ItemId, int32 Count, int32 SpecialSlot) {
    auto PlayerController = Cast<ACorePlayerController>(GetOwner());
    auto CorePlayerState = Cast<ACoreCharacterStateBase>(PlayerController->PlayerState);
    if (ensureMsgf(CorePlayerState, TEXT("CorePlayerState为空"))) {
        CorePlayerState->BackpackComponent->AddItem(BackpackType, ItemId, Count, TEXT("EditorTest"), SpecialSlot);
    }
}

void UGMComponent::UseItemInEditor(uint8 BackpackType, int SlotIndex, int Count) {
    auto PlayerController = Cast<ACorePlayerController>(GetOwner());
    auto CorePlayerState = Cast<ACoreCharacterStateBase>(PlayerController->PlayerState);
    if (ensureMsgf(CorePlayerState, TEXT("CorePlayerState为空"))) {
        if (CorePlayerState->BackpackComponent->Backpacks.IsValidIndex(BackpackType)) {
            auto Item = CorePlayerState->BackpackComponent->Backpacks[BackpackType].ItemList[SlotIndex];
            if (Item) {
                CorePlayerState->BackpackComponent->UseItem(BackpackType, Item->InstanceID, Count, TEXT("EditorTest"));
            }
        }
    }
}

void UGMComponent::AbandonItemInEditor(uint8 BackpackType, int SlotIndex, int Count) {
    auto PlayerController = Cast<ACorePlayerController>(GetOwner());
    auto CorePlayerState = Cast<ACoreCharacterStateBase>(PlayerController->PlayerState);
    if (ensureMsgf(CorePlayerState, TEXT("CorePlayerState为空"))) {
        if (CorePlayerState->BackpackComponent->Backpacks.IsValidIndex(BackpackType)) {
            auto Item = CorePlayerState->BackpackComponent->Backpacks[BackpackType].ItemList[SlotIndex];
            if (Item) {
                CorePlayerState->BackpackComponent->AbandonItem(BackpackType, Item->InstanceID, Count, TEXT("EditorTest"));
            }
        }
    }
}

void UGMComponent::DeductItemInEditor(uint8 BackpackType, int32 ItemId, int Count, int SpecialSlot) {
    auto PlayerController = Cast<ACorePlayerController>(GetOwner());
    auto CorePlayerState = Cast<ACoreCharacterStateBase>(PlayerController->PlayerState);
    if (ensureMsgf(CorePlayerState, TEXT("CorePlayerState为空"))) {
        FString SpecialInstanceID = "";
        if (SpecialSlot != -1) {
            if (CorePlayerState->BackpackComponent->Backpacks.IsValidIndex(BackpackType)) {
                auto Item = CorePlayerState->BackpackComponent->Backpacks[BackpackType].ItemList[SpecialSlot];
                if (Item) {
                    SpecialInstanceID = Item->InstanceID;
                }
            }
        }
        CorePlayerState->BackpackComponent->DeductItem(BackpackType, ItemId, Count, TEXT("EditorTest"), SpecialInstanceID);
    }
}

void UGMComponent::MoveItemInEditor(uint8 BackpackType, int SlotIndex, uint8 NewPackageType, int NewSlotIndex) {
    auto PlayerController = Cast<ACorePlayerController>(GetOwner());
    auto CorePlayerState = Cast<ACoreCharacterStateBase>(PlayerController->PlayerState);
    if (ensureMsgf(CorePlayerState, TEXT("CorePlayerState为空"))) {
        if (CorePlayerState->BackpackComponent->Backpacks.IsValidIndex(BackpackType)) {
            auto Item = CorePlayerState->BackpackComponent->Backpacks[BackpackType].ItemList[SlotIndex];
            if (Item) {
                CorePlayerState->BackpackComponent->MoveItem(BackpackType, Item->InstanceID, NewPackageType, NewSlotIndex);
            }
        }
    }
}

void UGMComponent::SplitItemInEditor(uint8 BackpackType, int SlotIndex, int Count) {
    auto PlayerController = Cast<ACorePlayerController>(GetOwner());
    auto CorePlayerState = Cast<ACoreCharacterStateBase>(PlayerController->PlayerState);
    if (ensureMsgf(CorePlayerState, TEXT("CorePlayerState为空"))) {
        if (CorePlayerState->BackpackComponent->Backpacks.IsValidIndex(BackpackType)) {
            auto Item = CorePlayerState->BackpackComponent->Backpacks[BackpackType].ItemList[SlotIndex];
            if (Item) {
                CorePlayerState->BackpackComponent->SplitItem(BackpackType, Item->InstanceID, Count);
            }
        }
    }
}

void UGMComponent::PickupItemInEditor(uint8 BackpackType, const FString& DropEntityId, int PickupCount) {
    auto PlayerController = Cast<ACorePlayerController>(GetOwner());
    auto GameInstance = PlayerController->GetGameInstance<UCoreGameInstance>();
    if (ensureMsgf(GameInstance, TEXT("CoreGameInstance为空"))) {
        auto CorePlayerState = Cast<ACoreCharacterStateBase>(PlayerController->PlayerState);
        if (ensureMsgf(CorePlayerState, TEXT("CorePlayerState为空"))) {
            auto SceneItem = Cast<ACoreSceneItem>(GameInstance->GameEntityManager->GetEntityById(DropEntityId).GetObject());
            if (SceneItem) {
                CorePlayerState->BackpackComponent->PickupItem(BackpackType, SceneItem, PickupCount);
            }
        }
    }
}

void UGMComponent::SortBackpack(uint8 BackpackType) {
    auto PlayerController = Cast<ACorePlayerController>(GetOwner());
    auto CorePlayerState = Cast<ACoreCharacterStateBase>(PlayerController->PlayerState);
    if (ensureMsgf(CorePlayerState, TEXT("CorePlayerState为空"))) {
        CorePlayerState->BackpackComponent->SortBackpack(BackpackType);
    }
}