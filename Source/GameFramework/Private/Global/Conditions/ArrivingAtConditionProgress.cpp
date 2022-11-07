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

TArray<TSubclassOf<class UGameEventBase>> UArrivingAtConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UEnterAreaEvent::StaticClass(),
		});
	}
}

bool UArrivingAtConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UEnterAreaEvent* EnterAreaEvent = (UEnterAreaEvent*)GameEvent;
	UArrivingAtCondition* ArrivingAtCondition = (UArrivingAtCondition*)Condition;
	if (EnterAreaEvent->Character->TemplateID == ArrivingAtCondition->UnitId
		&& EnterAreaEvent->AreaName == ArrivingAtCondition->LocationName) {
		HaveComplete = true;
		return true;
	}
	return false;
}

bool UArrivingAtConditionProgress::IsComplete_Implementation() {
	return HaveComplete;
}

void UArrivingAtConditionProgress::HandleComplete_Implementation() {

}

void UArrivingAtConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UArrivingAtConditionProgress, HaveComplete);
}