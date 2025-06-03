// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerQuestFinishCondition.h"
#include "CoreGameInstance.h"
#include "GameFrameworkUtils.h"
#include "CompleteQuestEvent.h"
#include "PlayerComponent.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "CoreCharacterStateBase.h"

UPlayerQuestFinishCondition::UPlayerQuestFinishCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerQuestFinishConditionProgress::StaticClass();
}

void UPlayerQuestFinishConditionProgress::OnInitialize() {
	Super::OnInitialize();

	HaveComplete = false;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerQuestFinishConditionProgress::GetHandleEventTypes() {
	EConditionSatisfy Satisfy = GetSatisfy();
	if (Satisfy == EConditionSatisfy::ECS_Satisfy) {
		return {};
	}
	else {
		return {
			UCompleteQuestEvent::StaticClass(),
		};
	}
}

void UPlayerQuestFinishConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
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

EConditionSatisfy UPlayerQuestFinishConditionProgress::GetSatisfy() {
	if (HaveComplete) {
		return EConditionSatisfy::ECS_Satisfy;
	}
	return EConditionSatisfy::ECS_NotSatisfy;
}

void UPlayerQuestFinishConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerQuestFinishConditionProgress, HaveComplete, Params);
}