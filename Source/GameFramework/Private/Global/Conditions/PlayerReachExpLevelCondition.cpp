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

bool UPlayerReachExpLevelConditionProgress::IsComplete(bool& IsValid) {
	IsValid = true;
	UPlayerReachExpLevelCondition* ReachExpLevelCondition = (UPlayerReachExpLevelCondition*)Condition;

	auto CharacterState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	auto ExpLevel = CharacterState->ExpComponent->GetExpLevel(ReachExpLevelCondition->ExpType);
	return ExpLevel >= ReachExpLevelCondition->ExpLevel;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerReachExpLevelConditionProgress::GetHandleEventTypes() {
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