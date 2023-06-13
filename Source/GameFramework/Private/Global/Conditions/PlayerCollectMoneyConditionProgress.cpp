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

bool UPlayerCollectMoneyConditionProgress::IsComplete_Implementation() {
	UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)Condition;
    auto PlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);

    return CollectMoneyCondition->MoneyCount <= PlayerState->WalletComponent->GetMoneyCount(CollectMoneyCondition->MoneyType);
}

void UPlayerCollectMoneyConditionProgress::HandleComplete_Implementation() {
	
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerCollectMoneyConditionProgress::GetHandleEventTypes_Implementation() {
    return { UChangeMoneyEvent::StaticClass() };
}

void UPlayerCollectMoneyConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
    UChangeMoneyEvent* ChangeMoneyEvent = (UChangeMoneyEvent*)HandleEvent;
    auto EventPlayerState = UGameFrameworkUtils::GetEntityState(ChangeMoneyEvent->Source);
    if (!EventPlayerState || !EventPlayerState->PlayerComponent) {
        return;
    }
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
        return;
    }
    UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)Condition;
    if (ChangeMoneyEvent->MoneyType == CollectMoneyCondition->MoneyType
        && EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {

        RefreshSatisfy();
    }
}