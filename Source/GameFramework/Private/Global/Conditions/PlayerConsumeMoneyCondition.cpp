// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Conditions/PlayerConsumeMoneyCondition.h"
#include "Global/Events/ConsumeMoneyEvent.h"
#include "Utils/GameFrameworkUtils.h"
#include "Modules/Player/PlayerComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/CoreCharacterStateBase.h"

UPlayerConsumeMoneyCondition::UPlayerConsumeMoneyCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerConsumeMoneyConditionProgress::StaticClass();
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerConsumeMoneyConditionProgress::GetHandleEventTypes() {
	EConditionSatisfy Satisfy = GetSatisfy();
	if (Satisfy == EConditionSatisfy::ECS_Satisfy) {
		return {};
	}
	else {
		return {
			UConsumeMoneyEvent::StaticClass(),
		};
	}
}

void UPlayerConsumeMoneyConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
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
    if (ConsumeMoneyEvent->MoneyType == ConsumeMoneyCondition->MoneyType
        && EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
        CurrentCount += ConsumeMoneyEvent->MoneyCount;
        
		RefreshSatisfy();
    }
}

EConditionSatisfy UPlayerConsumeMoneyConditionProgress::GetSatisfy() {
	UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)Condition;
	if (CurrentCount >= ConsumeMoneyCondition->MoneyCount) {
		return EConditionSatisfy::ECS_Satisfy;
	}
	return EConditionSatisfy::ECS_NotSatisfy;
}

void UPlayerConsumeMoneyConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerConsumeMoneyConditionProgress, CurrentCount, Params);
}