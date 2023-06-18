// Fill out your copyright notice in the Description page of Project Settings.

#include "ArrivingAtConditionProgress.h"
#include "ArrivingAtCondition.h"
#include "EnterAreaEvent.h"
#include "CoreCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

void UArrivingAtConditionProgress::PostProgressInitialize_Implementation() {
	HaveComplete = false;
}

bool UArrivingAtConditionProgress::IsComplete_Implementation() {
	return HaveComplete;
}

void UArrivingAtConditionProgress::HandleComplete_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UArrivingAtConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			UEnterAreaEvent::StaticClass(),
		};
	}
}

void UArrivingAtConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UEnterAreaEvent* EnterAreaEvent = (UEnterAreaEvent*)HandleEvent;
	UArrivingAtCondition* ArrivingAtCondition = (UArrivingAtCondition*)Condition;
	if (EnterAreaEvent->Character->TemplateID == ArrivingAtCondition->UnitIDContainer.UnitID
		&& EnterAreaEvent->AreaName == ArrivingAtCondition->LocationName) {
		HaveComplete = true;

		RefreshSatisfy();
	}
}

void UArrivingAtConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UArrivingAtConditionProgress, HaveComplete, Params);
}