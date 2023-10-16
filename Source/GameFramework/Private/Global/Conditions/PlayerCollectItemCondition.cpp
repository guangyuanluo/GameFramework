// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCollectItemCondition.h"
#include "GameFrameworkUtils.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "AssetSystem.h"
#include "BackpackComponent.h"
#include "CoreItem.h"
#include "ItemComponent.h"
#include "AssetBackpack.h"
#include "ChangeItemEvent.h"
#include "PlayerComponent.h"
#include "CoreCharacterStateBase.h"

UPlayerCollectItemCondition::UPlayerCollectItemCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerCollectItemConditionProgress::StaticClass();
}

void UPlayerCollectItemConditionProgress::OnUninitialize() {
    Super::OnUninitialize();

    UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)Condition;
    auto CharacterState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    UBackpackComponent* BackpackComponent = CharacterState->BackpackComponent;
    FString Error;
    auto GameInstance = ProgressOwner->GetWorld()->GetGameInstance<UCoreGameInstance>();
    GameInstance->GameSystemManager->GetSystemByClass<UAssetSystem>()->DeductItem(BackpackComponent, -1, CollectItemCondition->ItemIDContainer.ItemID, CollectItemCondition->ItemCount, -1, TEXT("ConditionComplete"), Error);
}

bool UPlayerCollectItemConditionProgress::IsComplete(bool& IsValid) {
    IsValid = true;
	UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)Condition;
    
    auto CharacterState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    check(CharacterState);
	int CurrentCount = 0;
    auto BackpackComponent = CharacterState->BackpackComponent;
    check(BackpackComponent);
    for (auto Index = 0; Index < BackpackComponent->Backpacks.Num(); ++Index) {
		for (int SlotIndex = 0; SlotIndex < BackpackComponent->Backpacks.Num(); ++SlotIndex) {
			if (BackpackComponent->Backpacks[Index].ItemList[SlotIndex] != nullptr) {
				CurrentCount += BackpackComponent->Backpacks[Index].ItemList[SlotIndex]->ItemNum;
			}
		}
	}
	return CurrentCount >= CollectItemCondition->ItemCount;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerCollectItemConditionProgress::GetHandleEventTypes() {
    return { UChangeItemEvent::StaticClass() };
}

void UPlayerCollectItemConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
    UChangeItemEvent* ChangeItemEvent = (UChangeItemEvent*)HandleEvent;

    auto SourcePlayerState = UGameFrameworkUtils::GetEntityState(ChangeItemEvent->Source);
    if (!SourcePlayerState || !SourcePlayerState->PlayerComponent) {
        return;
    }
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
        return;
    }
    UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)Condition;
    if (ChangeItemEvent->ItemId == CollectItemCondition->ItemIDContainer.ItemID
        && SourcePlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {

        RefreshSatisfy();
    }
}