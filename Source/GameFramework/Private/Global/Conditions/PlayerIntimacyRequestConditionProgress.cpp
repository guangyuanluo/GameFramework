// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerIntimacyRequestConditionProgress.h"
#include "PlayerIntimacyRequestCondition.h"
#include "IntimacyChangeEvent.h"
#include "GameFrameworkUtils.h"
#include "PlayerComponent.h"
#include "CoreCharacterStateBase.h"

void UPlayerIntimacyRequestConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UPlayerIntimacyRequestConditionProgress::GetCareEventTypes_Implementation() {
	return { UIntimacyChangeEvent::StaticClass() };
}

bool UPlayerIntimacyRequestConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UIntimacyChangeEvent* IntimacyChangeEvent = (UIntimacyChangeEvent*)GameEvent;
    auto EventPlayerState = UGameFrameworkUtils::GetEntityState(IntimacyChangeEvent->Source);
	if (EventPlayerState == nullptr && EventPlayerState->PlayerComponent == nullptr) {
		return false;
	}
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (ConditionPlayerState&& ConditionPlayerState->PlayerComponent) {
		UPlayerIntimacyRequestCondition* IntimacyRequestCondition = (UPlayerIntimacyRequestCondition*)Condition;
		if (IntimacyChangeEvent->NPCId == IntimacyRequestCondition->NPCId
			&& EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
			return IsComplete();
		}
	}
	return false;
}

bool UPlayerIntimacyRequestConditionProgress::IsComplete_Implementation() {
	return false;
}

void UPlayerIntimacyRequestConditionProgress::HandleComplete_Implementation() {

}