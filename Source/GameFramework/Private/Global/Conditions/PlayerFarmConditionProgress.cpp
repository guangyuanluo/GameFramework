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

bool UPlayerFarmConditionProgress::IsComplete_Implementation() {
	UPlayerFarmCondition* FarmCondition = (UPlayerFarmCondition*)Condition;
	return FinishCount >= FarmCondition->Count;
}

void UPlayerFarmConditionProgress::HandleComplete_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UPlayerFarmConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			UActorDiedEvent::StaticClass(),
		};
	}
}

void UPlayerFarmConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UActorDiedEvent* ActorDiedEvent = (UActorDiedEvent*)HandleEvent;
	UPlayerFarmCondition* FarmCondition = (UPlayerFarmCondition*)Condition;
	if (FarmCondition->UnitId != ActorDiedEvent->DiedUnit->TemplateID) {
		return;
	}
	auto KillPlayerState = UGameFrameworkUtils::GetEntityState(ActorDiedEvent->KillUnit);
	if (!KillPlayerState || !KillPlayerState->PlayerComponent) {
		return;
	}
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (ConditionPlayerState->PlayerComponent->RoleID == KillPlayerState->PlayerComponent->RoleID) {
		FinishCount += 1;

		RefreshSatisfy();
	}
}

void UPlayerFarmConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerFarmConditionProgress, FinishCount, Params);
}