// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerIntimacyRequestConditionProgress.h"
#include "PlayerIntimacyRequestCondition.h"
#include "IntimacyChangeEvent.h"
#include "GameFrameworkUtils.h"
#include "PlayerComponent.h"
#include "CoreCharacterStateBase.h"

bool UPlayerIntimacyRequestConditionProgress::IsComplete_Implementation() {
	return false;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerIntimacyRequestConditionProgress::GetHandleEventTypes_Implementation() {
	return { UIntimacyChangeEvent::StaticClass() };
}

void UPlayerIntimacyRequestConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UIntimacyChangeEvent* IntimacyChangeEvent = (UIntimacyChangeEvent*)HandleEvent;
	auto EventPlayerState = UGameFrameworkUtils::GetEntityState(IntimacyChangeEvent->Source);
	if (EventPlayerState == nullptr && EventPlayerState->PlayerComponent == nullptr) {
		return;
	}
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (ConditionPlayerState && ConditionPlayerState->PlayerComponent) {
		UPlayerIntimacyRequestCondition* IntimacyRequestCondition = (UPlayerIntimacyRequestCondition*)Condition;
		if (IntimacyChangeEvent->NPCId == IntimacyRequestCondition->NPCIDContainer.UnitID
			&& EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {

			RefreshSatisfy();
		}
	}
}