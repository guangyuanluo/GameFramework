// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerQuestFinishConditionProgress.h"
#include "PlayerQuestFinishCondition.h"
#include "CoreGameInstance.h"
#include "GameFrameworkUtils.h"
#include "CompleteQuestEvent.h"
#include "PlayerComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "CoreCharacterStateBase.h"

void UPlayerQuestFinishConditionProgress::PostProgressInitialize_Implementation() {
	HaveComplete = false;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerQuestFinishConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UCompleteQuestEvent::StaticClass(),
		});
	}
}

bool UPlayerQuestFinishConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UCompleteQuestEvent* CompleteQuestEvent = (UCompleteQuestEvent*)GameEvent;
    auto EventPlayerState = UGameFrameworkUtils::GetEntityState(CompleteQuestEvent->Source);
	if (EventPlayerState == nullptr && EventPlayerState->PlayerComponent) {
		return false;
	}
	UPlayerQuestFinishCondition* QuestFinishCondition = (UPlayerQuestFinishCondition*)Condition;
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (QuestFinishCondition->FinishQuestId == CompleteQuestEvent->CompleteQuestId
		&& EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
		if (CompleteQuestEvent->CompleteQuestId == QuestFinishCondition->FinishQuestId) {
			HaveComplete = true;
			return true;
		}
	}
	return false;
}

bool UPlayerQuestFinishConditionProgress::IsComplete_Implementation() {
	return HaveComplete;
}

void UPlayerQuestFinishConditionProgress::HandleComplete_Implementation() {

}

void UPlayerQuestFinishConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UPlayerQuestFinishConditionProgress, HaveComplete);
}