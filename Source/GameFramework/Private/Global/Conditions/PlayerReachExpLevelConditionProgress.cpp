// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerReachExpLevelConditionProgress.h"
#include "PlayerReachExpLevelCondition.h"
#include "GameFrameworkUtils.h"
#include "ExpComponent.h"
#include "Modules/Exp/ExpInfo.h"
#include "ExpLevelUpEvent.h"
#include "PlayerComponent.h"
#include "CoreCharacterStateBase.h"

bool UPlayerReachExpLevelConditionProgress::IsComplete_Implementation() {
	UPlayerReachExpLevelCondition* ReachExpLevelCondition = (UPlayerReachExpLevelCondition*)Condition;

    auto CharacterState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	auto ExpLevel = CharacterState->ExpComponent->GetExpLevel(ReachExpLevelCondition->ExpTypeContainer.ExpType);
	return ExpLevel >= ReachExpLevelCondition->ExpLevel;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerReachExpLevelConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			UExpLevelUpEvent::StaticClass(),
		};
	}
}

void UPlayerReachExpLevelConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
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
	if (ExpLevelUpEvent->ExpTypeId == ReachExpLevelCondition->ExpTypeContainer.ExpType
		&& EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
		RefreshSatisfy();
	}
}