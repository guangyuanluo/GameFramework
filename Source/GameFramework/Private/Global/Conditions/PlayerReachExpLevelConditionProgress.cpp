// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerReachExpLevelConditionProgress.h"
#include "PlayerReachExpLevelCondition.h"
#include "GameFrameworkUtils.h"
#include "ExpComponent.h"
#include "Modules/Exp/ExpInfo.h"
#include "IncreaseExpLevelEvent.h"
#include "PlayerComponent.h"
#include "CoreCharacterStateBase.h"

void UPlayerReachExpLevelConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UPlayerReachExpLevelConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UIncreaseExpLevelEvent::StaticClass(),
		});
	}
}

bool UPlayerReachExpLevelConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UIncreaseExpLevelEvent* IncreaseExpLevelEvent = (UIncreaseExpLevelEvent*)GameEvent;
    auto EventPlayerState = UGameFrameworkUtils::GetEntityState(IncreaseExpLevelEvent->Source);
	if (!EventPlayerState || !EventPlayerState->PlayerComponent) {
		return false;
	}
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
        return false;
    }
    UPlayerReachExpLevelCondition* ReachExpLevelCondition = (UPlayerReachExpLevelCondition*)Condition;
    if (IncreaseExpLevelEvent->ExpTypeId == ReachExpLevelCondition->ExpType
        && EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
        return true;
    }
	return false;
}

bool UPlayerReachExpLevelConditionProgress::IsComplete_Implementation() {
	UPlayerReachExpLevelCondition* ReachExpLevelCondition = (UPlayerReachExpLevelCondition*)Condition;

    auto CharacterState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	auto ExpLevel = CharacterState->ExpComponent->GetExpLevel(ReachExpLevelCondition->ExpType);
	return ExpLevel >= ReachExpLevelCondition->ExpLevel;
}

void UPlayerReachExpLevelConditionProgress::HandleComplete_Implementation() {

}