// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCollectMoneyConditionProgress.h"
#include "PlayerCollectMoneyCondition.h"
#include "CoreGameInstance.h"
#include "GameFrameworkUtils.h"
#include "AssetSystem.h"
#include "WalletComponent.h"
#include "ChangeMoneyEvent.h"
#include "PlayerComponent.h"
#include "CoreCharacterStateBase.h"

void UPlayerCollectMoneyConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UPlayerCollectMoneyConditionProgress::GetCareEventTypes_Implementation() {
	return { UChangeMoneyEvent::StaticClass() };
}

bool UPlayerCollectMoneyConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UChangeMoneyEvent* ChangeMoneyEvent = (UChangeMoneyEvent*)GameEvent;
	auto EventPlayerState = UGameFrameworkUtils::GetEntityState(ChangeMoneyEvent->Source);
	if (!EventPlayerState || !EventPlayerState->PlayerComponent) {
		return false;
	}
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
        return false;
    }
    UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)Condition;
    if (ChangeMoneyEvent->MoneyType == CollectMoneyCondition->MoneyType
        && EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
        return true;
    }
	return false;
}

bool UPlayerCollectMoneyConditionProgress::IsComplete_Implementation() {
	UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)Condition;
    auto PlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);

    return CollectMoneyCondition->MoneyCount <= PlayerState->WalletComponent->GetMoneyCount(CollectMoneyCondition->MoneyType);
}

void UPlayerCollectMoneyConditionProgress::HandleComplete_Implementation() {
	
}