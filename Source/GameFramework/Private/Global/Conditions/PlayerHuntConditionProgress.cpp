// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHuntConditionProgress.h"
#include "PlayerHuntCondition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

bool UPlayerHuntConditionProgress::IsComplete_Implementation() {
	UPlayerHuntCondition* huntCondition = (UPlayerHuntCondition*)Condition;
	return ContributionDegree >= huntCondition->ContributionDegree;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerHuntConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
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