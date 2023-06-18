// Fill out your copyright notice in the Description page of Project Settings.

#include "AcceptableQuest.h"
#include "CoreConditionProgress.h"
#include "QuestSystem.h"
#include "QuestComponent.h"
#include "UE4LogImpl.h"
#include "Quest.h"
#include "Kismet/GameplayStatics.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "ConditionSystem.h"

void UAcceptableQuest::Initialize(UQuest* InQuestPtr, UQuestComponent* InQuestComponent) {
	QuestPtr = InQuestPtr;
	QuestComponent = InQuestComponent;

	//未接受的任务
	for (auto PreCondition : QuestPtr->PreConditionList.Conditions) {
		auto ConditionProgress = PreCondition->GenerateConditionProgress(QuestComponent->GetOwner());
		QuestProgresses.Add(ConditionProgress);
	}
}

void UAcceptableQuest::StartListen() {
	auto GameInstance = Cast<UCoreGameInstance>(UGameplayStatics::GetGameInstance(QuestComponent));
	GameInstance->GameSystemManager->GetSystemByClass<UConditionSystem>()->FollowConditions(QuestProgresses, this);
}

void UAcceptableQuest::StopListen() {
	Unfollow();
}

const FGuid& UAcceptableQuest::GetID() const {
	return QuestPtr->ID;
}

const TArray<UCoreConditionProgress*>& UAcceptableQuest::GetQuestProgresses() const {
	return QuestProgresses;
}

bool UAcceptableQuest::IsComplete() const {
	bool isComplete = true;
	for (int Index = 0; Index < QuestProgresses.Num(); ++Index) {
		if (!QuestProgresses[Index]->IsComplete()) {
			isComplete = false;
			break;
		}
	}
	return isComplete;
}

void UAcceptableQuest::OnSatisfyConditions_Implementation(const TArray<UCoreConditionProgress*>& Progresses) {
	//所有任务进度都变成完成，接受任务，开启后续流程
	QuestComponent->AcceptQuest(GetID());
}

void UAcceptableQuest::OnProgressRefresh_Implementation(UCoreConditionProgress* ChangeProgress) {
	
}