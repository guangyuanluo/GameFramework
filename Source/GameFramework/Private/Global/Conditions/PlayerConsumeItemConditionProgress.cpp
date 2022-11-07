// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerConsumeItemConditionProgress.h"
#include "PlayerConsumeItemCondition.h"
#include "ChangeItemEvent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "GameFrameworkUtils.h"
#include "PlayerComponent.h"
#include "CoreCharacterStateBase.h"

void UPlayerConsumeItemConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UPlayerConsumeItemConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UChangeItemEvent::StaticClass(),
		});
	}
}

bool UPlayerConsumeItemConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UChangeItemEvent* ChangeItemEvent = (UChangeItemEvent*)GameEvent;
	auto EventPlayerState = UGameFrameworkUtils::GetEntityState(ChangeItemEvent->Source);
	if (!EventPlayerState || !EventPlayerState->PlayerComponent) {
		return false;
	}
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
		return false;
	}
	UPlayerConsumeItemCondition* PlayerConsumeItemCondition = (UPlayerConsumeItemCondition*)Condition;
	if (ChangeItemEvent->ItemId == PlayerConsumeItemCondition->ItemId
		&& EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID
		&& ChangeItemEvent->Count < 0) {
		CurrentCount += -ChangeItemEvent->Count;
		return true;
	}
	return false;
}

bool UPlayerConsumeItemConditionProgress::IsComplete_Implementation() {
	UPlayerConsumeItemCondition* ConsumeItemCondition = (UPlayerConsumeItemCondition*)Condition;
	return CurrentCount >= ConsumeItemCondition->ItemCount;
}

void UPlayerConsumeItemConditionProgress::HandleComplete_Implementation() {
	
}

void UPlayerConsumeItemConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UPlayerConsumeItemConditionProgress, CurrentCount);
}