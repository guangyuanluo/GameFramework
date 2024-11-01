// Fill out your copyright notice in the Description page of Project Settings.

#include "ArrivingAtCondition.h"
#include "EnterAreaEvent.h"
#include "ExitAreaEvent.h"
#include "CoreCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "CoreCharacterStateBase.h"
#include "GameArea.h"

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

bool UArrivingAtConditionProgress::IsComplete(bool& IsValid) {
	IsValid = true;
	return HaveComplete;
}

TArray<TSubclassOf<class UGameEventBase>> UArrivingAtConditionProgress::GetHandleEventTypes() {
	bool IsValid;
	bool bComplete = IsComplete(IsValid);
	if (!IsValid) {
		return {};
	}
	if (bComplete) {
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