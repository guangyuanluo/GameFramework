// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerConsumeMoneyConditionProgress.h"
#include "PlayerConsumeMoneyCondition.h"
#include "ConsumeMoneyEvent.h"
#include "GameFrameworkUtils.h"
#include "PlayerComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "CoreCharacterStateBase.h"

TArray<TSubclassOf<class UGameEventBase>> UPlayerConsumeMoneyConditionProgress::GetHandleEventTypes_Implementation() {
	bool IsValid;
	bool bComplete = IsComplete(IsValid);
	if (!IsValid) {
		return {};
	}
	if (bComplete) {
		return {};
	}
	else {
		return {
			UConsumeMoneyEvent::StaticClass(),
		};
	}
}

void UPlayerConsumeMoneyConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UConsumeMoneyEvent* ConsumeMoneyEvent = (UConsumeMoneyEvent*)HandleEvent;
	auto EventPlayerState = UGameFrameworkUtils::GetEntityState(ConsumeMoneyEvent->Source);
	if (!EventPlayerState || !EventPlayerState->PlayerComponent) {
		return;
	}
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
        return;
    }
    UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)Condition;
    if (ConsumeMoneyEvent->MoneyType == ConsumeMoneyCondition->MoneyTypeContainer.MoneyType
        && EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
        CurrentCount += ConsumeMoneyEvent->MoneyCount;
        
		RefreshSatisfy();
    }
}

bool UPlayerConsumeMoneyConditionProgress::IsComplete_Implementation(bool& IsValid) {
	IsValid = true;
	UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)Condition;
	return CurrentCount >= ConsumeMoneyCondition->MoneyCount;
}

void UPlayerConsumeMoneyConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerConsumeMoneyConditionProgress, CurrentCount, Params);
}