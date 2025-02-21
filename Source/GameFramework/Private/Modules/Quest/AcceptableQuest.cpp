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
	auto ProgressOwner = QuestComponent->GetOwner();
	for (auto PreCondition : QuestPtr->PreConditionList) {
		auto ConditionProgress = PreCondition->GenerateConditionProgress(ProgressOwner);
		ConditionProgress->Initialize();
		QuestProgresses.Add(ConditionProgress);
	}
}

void UAcceptableQuest::Uninitialize() {
	for (auto QuestProgress : QuestProgresses) {
		QuestProgress->Uninitialize();
	}
	QuestProgresses.Empty();
}

void UAcceptableQuest::StartListen() {
	FOnAllProgressesSatisfyDelegate Callback;
	Callback.BindUFunction(this, TEXT("OnAllProgressesSatisfy"));

	auto GameInstance = Cast<UCoreGameInstance>(UGameplayStatics::GetGameInstance(QuestComponent));
	GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>()->FollowConditions(ConditionTriggerHandler, QuestProgresses, Callback);
}

void UAcceptableQuest::StopListen() {
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

void UAcceptableQuest::OnAllProgressesSatisfy(FConditionTriggerHandler Handler) {
	//所有任务进度都变成完成，接受任务，开启后续流程
	QuestComponent->AcceptQuest(GetID());
}