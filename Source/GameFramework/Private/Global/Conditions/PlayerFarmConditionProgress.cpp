// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerFarmConditionProgress.h"
#include "PlayerFarmCondition.h"
#include "ActorDiedEvent.h"
#include "GameFrameworkUtils.h"
#include "PlayerComponent.h"
#include "CoreCharacter.h"
#include "CorePlayerController.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "CoreCharacterStateBase.h"

void UPlayerFarmConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UPlayerFarmConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UActorDiedEvent::StaticClass(),
		});
	}
}

bool UPlayerFarmConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UActorDiedEvent* ActorDiedEvent = (UActorDiedEvent*)GameEvent;
	UPlayerFarmCondition* FarmCondition = (UPlayerFarmCondition*)Condition;
	if (FarmCondition->UnitId != ActorDiedEvent->DiedUnit->TemplateID) {
		return false;
	}
	auto KillPlayerState = UGameFrameworkUtils::GetEntityState(ActorDiedEvent->KillUnit);
	if (!KillPlayerState || !KillPlayerState->PlayerComponent) {
		return false;
	}
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (ConditionPlayerState->PlayerComponent->RoleID == KillPlayerState->PlayerComponent->RoleID) {
		FinishCount += 1;
		return true;
	}
    return false;
}

bool UPlayerFarmConditionProgress::IsComplete_Implementation() {
	UPlayerFarmCondition* FarmCondition = (UPlayerFarmCondition*)Condition;
	return FinishCount >= FarmCondition->Count;
}

void UPlayerFarmConditionProgress::HandleComplete_Implementation() {

}

void UPlayerFarmConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UPlayerFarmConditionProgress, FinishCount);
}