// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Conditions/PlayerFarmCondition.h"
#include "Global/Events/ActorDiedEvent.h"
#include "Utils/GameFrameworkUtils.h"
#include "Modules/Player/PlayerComponent.h"
#include "Character/CoreCharacter.h"
#include "PlayerController/CorePlayerController.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/CoreCharacterStateBase.h"

UPlayerFarmCondition::UPlayerFarmCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerFarmConditionProgress::StaticClass();
}

EConditionSatisfy UPlayerFarmConditionProgress::GetSatisfy() {
	UPlayerFarmCondition* FarmCondition = (UPlayerFarmCondition*)Condition;
	if (FinishCount >= FarmCondition->Count) {
		return EConditionSatisfy::ECS_Satisfy;
	}
	return EConditionSatisfy::ECS_NotSatisfy;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerFarmConditionProgress::GetHandleEventTypes() {
	EConditionSatisfy Satisfy = GetSatisfy();
	if (Satisfy == EConditionSatisfy::ECS_Satisfy) {
		return {};
	}
	else {
		return {
			UActorDiedEvent::StaticClass(),
		};
	}
}

void UPlayerFarmConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UActorDiedEvent* ActorDiedEvent = (UActorDiedEvent*)HandleEvent;
	UPlayerFarmCondition* FarmCondition = (UPlayerFarmCondition*)Condition;
	if (FarmCondition->UnitIDContainer.UnitID != ActorDiedEvent->DiedUnit->TemplateID) {
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