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
#include "ConditionTriggerSystem.h"
#include "ConditionBlueprintLibrary.h"

void UAcceptableQuest::Initialize(UQuest* InQuestPtr, UQuestComponent* InQuestComponent) {
	QuestPtr = InQuestPtr;
	QuestComponent = InQuestComponent;

	//未接受的任务
	for (auto PreCondition : QuestPtr->PreConditionList.Conditions) {
		auto ConditionProgress = PreCondition->GenerateConditionProgress(QuestComponent->GetOwner());
		ConditionProgress->OnInitialize();
		QuestProgresses.Add(ConditionProgress);
	}
}

void UAcceptableQuest::Uninitialize() {
	for (auto QuestProgress : QuestProgresses) {
		QuestProgress->OnUninitialize();
	}
}

void UAcceptableQuest::StartListen() {
	ConditionTriggerHandler.OnAllProgressesSatisfy.AddUObject(this, &UAcceptableQuest::OnAllProgressesSatisfy);

	auto GameInstance = Cast<UCoreGameInstance>(UGameplayStatics::GetGameInstance(QuestComponent));
	GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>()->FollowConditions(ConditionTriggerHandler, QuestProgresses);
}

void UAcceptableQuest::StopListen() {
	ConditionTriggerHandler.OnAllProgressesSatisfy.RemoveAll(this);

	auto GameInstance = Cast<UCoreGameInstance>(UGameplayStatics::GetGameInstance(QuestComponent));
	GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>()->UnfollowConditions(ConditionTriggerHandler);
}

const FGuid& UAcceptableQuest::GetID() const {
	return QuestPtr->ID;
}

const TArray<UCoreConditionProgress*>& UAcceptableQuest::GetQuestProgresses() const {
	return QuestProgresses;
}

bool UAcceptableQuest::IsComplete() const {
	return UConditionBlueprintLibrary::DoesProgressesSatisfy(QuestProgresses);
}

void UAcceptableQuest::OnAllProgressesSatisfy() {
	//所有任务进度都变成完成，接受任务，开启后续流程
	QuestComponent->AcceptQuest(GetID());
}