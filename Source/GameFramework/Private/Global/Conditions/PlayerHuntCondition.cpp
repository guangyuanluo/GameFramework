// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHuntCondition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UPlayerHuntCondition::UPlayerHuntCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerHuntConditionProgress::StaticClass();
}

EConditionSatisfy UPlayerHuntConditionProgress::GetSatisfy() {
	UPlayerHuntCondition* huntCondition = (UPlayerHuntCondition*)Condition;
	if (ContributionDegree >= huntCondition->ContributionDegree) {
		return EConditionSatisfy::ECS_Satisfy;
	}
	return EConditionSatisfy::ECS_NotSatisfy;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerHuntConditionProgress::GetHandleEventTypes() {
	return {};
}

void UPlayerHuntConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {

}

void UPlayerHuntConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerHuntConditionProgress, ContributionDegree, Params);
}