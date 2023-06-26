// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerConsumeItemConditionProgress.h"
#include "PlayerConsumeItemCondition.h"
#include "ChangeItemEvent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "GameFrameworkUtils.h"
#include "PlayerComponent.h"
#include "CoreCharacterStateBase.h"

TArray<TSubclassOf<class UGameEventBase>> UPlayerConsumeItemConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			UChangeItemEvent::StaticClass(),
		};
	}
}

void UPlayerConsumeItemConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
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

bool UPlayerConsumeItemConditionProgress::IsComplete_Implementation() {
	UPlayerConsumeItemCondition* ConsumeItemCondition = (UPlayerConsumeItemCondition*)Condition;
	return CurrentCount >= ConsumeItemCondition->ItemCount;
}

void UPlayerConsumeItemConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerConsumeItemConditionProgress, CurrentCount, Params);
}