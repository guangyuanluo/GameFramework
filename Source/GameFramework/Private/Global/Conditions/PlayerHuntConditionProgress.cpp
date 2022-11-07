// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHuntConditionProgress.h"
#include "PlayerHuntCondition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

void UPlayerHuntConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UPlayerHuntConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
}

bool UPlayerHuntConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	return false;
}

bool UPlayerHuntConditionProgress::IsComplete_Implementation() {
	UPlayerHuntCondition* huntCondition = (UPlayerHuntCondition*)Condition;
	return ContributionDegree >= huntCondition->ContributionDegree;
}

void UPlayerHuntConditionProgress::HandleComplete_Implementation() {

}

void UPlayerHuntConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UPlayerHuntConditionProgress, ContributionDegree);
}