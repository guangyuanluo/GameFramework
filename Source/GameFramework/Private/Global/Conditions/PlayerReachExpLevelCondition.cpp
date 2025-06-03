// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerReachExpLevelCondition.h"
#include "GameFrameworkUtils.h"
#include "ExpComponent.h"
#include "Modules/Exp/ExpInfo.h"
#include "ExpLevelUpEvent.h"
#include "PlayerComponent.h"
#include "CoreCharacterStateBase.h"

UPlayerReachExpLevelCondition::UPlayerReachExpLevelCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerReachExpLevelConditionProgress::StaticClass();
}

EConditionSatisfy UPlayerReachExpLevelConditionProgress::GetSatisfy() {
	UPlayerReachExpLevelCondition* ReachExpLevelCondition = (UPlayerReachExpLevelCondition*)Condition;

	auto CharacterState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	auto ExpLevel = CharacterState->ExpComponent->GetExpLevel(ReachExpLevelCondition->ExpType);
	if (ExpLevel >= ReachExpLevelCondition->ExpLevel) {
		return EConditionSatisfy::ECS_Satisfy;
	}
	return EConditionSatisfy::ECS_NotSatisfy;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerReachExpLevelConditionProgress::GetHandleEventTypes() {
	EConditionSatisfy Satisfy = GetSatisfy();
	if (Satisfy == EConditionSatisfy::ECS_Satisfy) {
		return {};
	}
	else {
		return {
			UExpLevelUpEvent::StaticClass(),
		};
	}
}

void UPlayerReachExpLevelConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UExpLevelUpEvent* ExpLevelUpEvent = (UExpLevelUpEvent*)HandleEvent;
	auto EventPlayerState = UGameFrameworkUtils::GetEntityState(ExpLevelUpEvent->Source);
	if (!EventPlayerState || !EventPlayerState->PlayerComponent) {
		return;
	}
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
		return;
	}
	UPlayerReachExpLevelCondition* ReachExpLevelCondition = (UPlayerReachExpLevelCondition*)Condition;
	if (ExpLevelUpEvent->ExpType == ReachExpLevelCondition->ExpType
		&& EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
		RefreshSatisfy();
	}
}