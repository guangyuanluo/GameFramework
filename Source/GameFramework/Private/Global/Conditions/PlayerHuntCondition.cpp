// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHuntCondition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UPlayerHuntCondition::UPlayerHuntCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerHuntConditionProgress::StaticClass();
}

bool UPlayerHuntConditionProgress::IsComplete_Implementation(bool& IsValid) {
	IsValid = true;
	UPlayerHuntCondition* huntCondition = (UPlayerHuntCondition*)Condition;
	return ContributionDegree >= huntCondition->ContributionDegree;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerHuntConditionProgress::GetHandleEventTypes_Implementation() {
	bool IsValid;
	bool bComplete = IsComplete(IsValid);
	if (!IsValid) {
		return {};
	}
	if (bComplete) {
		return {};
	}
	else {
		return {};
	}
}

void UPlayerHuntConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {

}

void UPlayerHuntConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerHuntConditionProgress, ContributionDegree, Params);
}