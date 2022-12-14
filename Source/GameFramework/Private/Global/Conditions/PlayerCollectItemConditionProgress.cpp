// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCollectItemConditionProgress.h"
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

void UPlayerCollectItemConditionProgress::PostProgressInitialize_Implementation() {
	
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerCollectItemConditionProgress::GetCareEventTypes_Implementation() {
	return { UChangeItemEvent::StaticClass() };
}

bool UPlayerCollectItemConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UChangeItemEvent* ChangeItemEvent = (UChangeItemEvent*)GameEvent;

    auto SourcePlayerState = UGameFrameworkUtils::GetEntityState(ChangeItemEvent->Source);
	if (!SourcePlayerState || !SourcePlayerState->PlayerComponent) {
		return false;
	}
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
        return false;
    }
    UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)Condition;
    if (ChangeItemEvent->ItemId == CollectItemCondition->ItemId
        && SourcePlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
        return true;
    }
	return false;
}

bool UPlayerCollectItemConditionProgress::IsComplete_Implementation() {
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

void UPlayerCollectItemConditionProgress::HandleComplete_Implementation() {
	UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)Condition;
    auto CharacterState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    UBackpackComponent* BackpackComponent = CharacterState->BackpackComponent;
	FString Error;
    auto GameInstance = ProgressOwner->GetWorld()->GetGameInstance<UCoreGameInstance>();
    GameInstance->GameSystemManager->GetSystemByClass<UAssetSystem>()->DeductItem(BackpackComponent, -1, CollectItemCondition->ItemId, CollectItemCondition->ItemCount, -1, TEXT("ConditionComplete"), Error);
}