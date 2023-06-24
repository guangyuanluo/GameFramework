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

void UPlayerQuestFinishConditionProgress::OnStart_Implementation() {
	Super::OnStart_Implementation();

	HaveComplete = false;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerQuestFinishConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			UCompleteQuestEvent::StaticClass(),
		};
	}
}

void UPlayerQuestFinishConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UCompleteQuestEvent* CompleteQuestEvent = (UCompleteQuestEvent*)HandleEvent;
    auto EventPlayerState = UGameFrameworkUtils::GetEntityState(CompleteQuestEvent->Source);
	if (EventPlayerState == nullptr && EventPlayerState->PlayerComponent) {
		return;
	}
	UPlayerQuestFinishCondition* QuestFinishCondition = (UPlayerQuestFinishCondition*)Condition;
    auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (QuestFinishCondition->FinishQuestId == CompleteQuestEvent->CompleteQuestId
		&& EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
		if (CompleteQuestEvent->CompleteQuestId == QuestFinishCondition->FinishQuestId) {
			HaveComplete = true;

			RefreshSatisfy();
		}
	}
}

bool UPlayerQuestFinishConditionProgress::IsComplete_Implementation() {
	return HaveComplete;
}

void UPlayerQuestFinishConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerQuestFinishConditionProgress, HaveComplete, Params);
}