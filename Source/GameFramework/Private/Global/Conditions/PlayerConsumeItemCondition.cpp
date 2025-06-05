// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Conditions/PlayerConsumeItemCondition.h"
#include "Global/Events/ChangeItemEvent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Utils/GameFrameworkUtils.h"
#include "Modules/Player/PlayerComponent.h"
#include "PlayerState/CoreCharacterStateBase.h"

UPlayerConsumeItemCondition::UPlayerConsumeItemCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerConsumeItemConditionProgress::StaticClass();
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerConsumeItemConditionProgress::GetHandleEventTypes() {
	EConditionSatisfy Satisfy = GetSatisfy();
	if (Satisfy == EConditionSatisfy::ECS_Satisfy) {
		return {};
	}
	else {
		return {
			UChangeItemEvent::StaticClass(),
		};
	}
}

void UPlayerConsumeItemConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UChangeItemEvent* ChangeItemEvent = (UChangeItemEvent*)HandleEvent;
	auto EventPlayerState = UGameFrameworkUtils::GetEntityState(ChangeItemEvent->Source);
	if (!EventPlayerState || !EventPlayerState->PlayerComponent) {
		return;
	}
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
		return;
	}
	UPlayerConsumeItemCondition* PlayerConsumeItemCondition = (UPlayerConsumeItemCondition*)Condition;
	if (ChangeItemEvent->ItemId == PlayerConsumeItemCondition->ItemIDContainer.ItemID
		&& EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID
		&& ChangeItemEvent->Count < 0) {
		CurrentCount += -ChangeItemEvent->Count;
		
		RefreshSatisfy();
	}
}

EConditionSatisfy UPlayerConsumeItemConditionProgress::GetSatisfy() {
	UPlayerConsumeItemCondition* ConsumeItemCondition = (UPlayerConsumeItemCondition*)Condition;
	if (CurrentCount >= ConsumeItemCondition->ItemCount) {
        return EConditionSatisfy::ECS_Satisfy;
    }
	return EConditionSatisfy::ECS_NotSatisfy;
}

void UPlayerConsumeItemConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerConsumeItemConditionProgress, CurrentCount, Params);
}