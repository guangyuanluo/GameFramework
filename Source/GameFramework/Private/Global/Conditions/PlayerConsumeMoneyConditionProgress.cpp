// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerConsumeMoneyConditionProgress.h"
#include "PlayerConsumeMoneyCondition.h"
#include "ConsumeMoneyEvent.h"
#include "GameFrameworkUtils.h"
#include "PlayerComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "CoreCharacterStateBase.h"

void UPlayerConsumeMoneyConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UPlayerConsumeMoneyConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UConsumeMoneyEvent::StaticClass(),
		});
	}
}

bool UPlayerConsumeMoneyConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UConsumeMoneyEvent* ConsumeMoneyEvent = (UConsumeMoneyEvent*)GameEvent;
	auto EventPlayerState = UGameFrameworkUtils::GetEntityState(ConsumeMoneyEvent->Source);
	if (!EventPlayerState || !EventPlayerState->PlayerComponent) {
		return false;
	}
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
        return false;
    }
    UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)Condition;
    if (ConsumeMoneyEvent->MoneyType == ConsumeMoneyCondition->MoneyType
        && EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
        CurrentCount += ConsumeMoneyEvent->MoneyCount;
        return true;
    }
	return false;
}

bool UPlayerConsumeMoneyConditionProgress::IsComplete_Implementation() {
	UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)Condition;
	return CurrentCount >= ConsumeMoneyCondition->MoneyCount;
}

void UPlayerConsumeMoneyConditionProgress::HandleComplete_Implementation() {

}

void UPlayerConsumeMoneyConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UPlayerConsumeMoneyConditionProgress, CurrentCount);
}