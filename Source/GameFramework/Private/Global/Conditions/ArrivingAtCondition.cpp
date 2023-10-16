// Fill out your copyright notice in the Description page of Project Settings.

#include "ArrivingAtCondition.h"
#include "EnterAreaEvent.h"
#include "CoreCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UArrivingAtCondition::UArrivingAtCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UArrivingAtConditionProgress::StaticClass();
}

void UArrivingAtConditionProgress::OnInitialize() {
	Super::OnInitialize();

	HaveComplete = false;
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
		return {};
	}
	else {
		return {
			UEnterAreaEvent::StaticClass(),
		};
	}
}

void UArrivingAtConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
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