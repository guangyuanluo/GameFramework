// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Conditions/ArrivingAtCondition.h"
#include "Global/Events/EnterAreaEvent.h"
#include "Global/Events/ExitAreaEvent.h"
#include "Character/CoreCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/CoreCharacterStateBase.h"
#include "Data/Area/GameArea.h"

UArrivingAtCondition::UArrivingAtCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UArrivingAtConditionProgress::StaticClass();
}

void UArrivingAtConditionProgress::OnInitialize() {
	Super::OnInitialize();

	HaveComplete = false;

	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (ConditionPlayerState) {
		auto Character = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());
		TArray<AActor*> OverlapAreas;
		Character->GetOverlappingActors(OverlapAreas, AGameArea::StaticClass());

		if (OverlapAreas.Num()) {
			UArrivingAtCondition* ArrivingAtCondition = (UArrivingAtCondition*)Condition;
			for (auto OverlapActor : OverlapAreas) {
				if (auto OverlapArea = Cast<AGameArea>(OverlapActor)) {
					if (OverlapArea->AreaName == ArrivingAtCondition->LocationName) {
						HaveComplete = true;
						break;
					}
				}
			}
		}
	}
}

EConditionSatisfy UArrivingAtConditionProgress::GetSatisfy() {
	if (HaveComplete) {
		return EConditionSatisfy::ECS_Satisfy;
	}
	return EConditionSatisfy::ECS_NotSatisfy;
}

TArray<TSubclassOf<class UGameEventBase>> UArrivingAtConditionProgress::GetHandleEventTypes() {
	EConditionSatisfy Satisfy = GetSatisfy();
	if (Satisfy == EConditionSatisfy::ECS_Satisfy) {
		return {
			UExitAreaEvent::StaticClass(),
		};
	}
	else {
		return {
			UEnterAreaEvent::StaticClass(),
		};
	}
}

void UArrivingAtConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	if (UEnterAreaEvent* EnterAreaEvent = Cast<UEnterAreaEvent>(HandleEvent)) {
		UArrivingAtCondition* ArrivingAtCondition = (UArrivingAtCondition*)Condition;
		if (ArrivingAtCondition->UnitIDContainer.UnitID == 0) {
			auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
			if (EnterAreaEvent->Character->GetPlayerState() != ConditionPlayerState) {
				return;
			}
		}
		else {
			if (EnterAreaEvent->Character->TemplateID == ArrivingAtCondition->UnitIDContainer.UnitID) {
				return;
			}
		}
		if (EnterAreaEvent->AreaName == ArrivingAtCondition->LocationName) {
			HaveComplete = true;

			RefreshSatisfy();
		}
	}
	else if (UExitAreaEvent* ExitAreaEvent = Cast<UExitAreaEvent>(HandleEvent)) {
		UArrivingAtCondition* ArrivingAtCondition = (UArrivingAtCondition*)Condition;
		if (ArrivingAtCondition->UnitIDContainer.UnitID == 0) {
			auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
			if (ExitAreaEvent->Character->GetPlayerState() != ConditionPlayerState) {
				return;
			}
		}
		else {
			if (ExitAreaEvent->Character->TemplateID == ArrivingAtCondition->UnitIDContainer.UnitID) {
				return;
			}
		}
		if (ExitAreaEvent->AreaName == ArrivingAtCondition->LocationName) {
			HaveComplete = false;

			RefreshSatisfy();
		}
	}
}

void UArrivingAtConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UArrivingAtConditionProgress, HaveComplete, Params);
}