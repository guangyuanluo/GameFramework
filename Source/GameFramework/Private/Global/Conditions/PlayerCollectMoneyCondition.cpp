// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Conditions/PlayerCollectMoneyCondition.h"
#include "GameInstance/CoreGameInstance.h"
#include "Utils/GameFrameworkUtils.h"
#include "Modules/Assets/AssetSystem.h"
#include "Modules/Assets/WalletComponent.h"
#include "Global/Events/ChangeMoneyEvent.h"
#include "Modules/Player/PlayerComponent.h"
#include "PlayerState/CoreCharacterStateBase.h"

UPlayerCollectMoneyCondition::UPlayerCollectMoneyCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerCollectMoneyConditionProgress::StaticClass();
}

EConditionSatisfy UPlayerCollectMoneyConditionProgress::GetSatisfy() {
	UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)Condition;
    auto PlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);

    if (CollectMoneyCondition->MoneyCount <= PlayerState->WalletComponent->GetMoneyCount(CollectMoneyCondition->MoneyType)) {
        return EConditionSatisfy::ECS_Satisfy;
    }
    return EConditionSatisfy::ECS_NotSatisfy;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerCollectMoneyConditionProgress::GetHandleEventTypes() {
    return { UChangeMoneyEvent::StaticClass() };
}

void UPlayerCollectMoneyConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
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