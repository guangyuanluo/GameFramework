// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreConditionProgress_Event.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "EventSystem.h"

void UCoreConditionProgress_Event::OnStart_Implementation() {
    Super::OnStart_Implementation();

    auto GameInstance = ProgressOwner->GetGameInstance<UCoreGameInstance>();
    auto EventSystem = GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
    EventSystem->RegistEventHandler(this);
}

void UCoreConditionProgress_Event::OnEnd_Implementation() {
    Super::OnEnd_Implementation();

    auto GameInstance = ProgressOwner->GetGameInstance<UCoreGameInstance>();
    auto EventSystem = GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
    EventSystem->UnRegistEventHandler(this);
}

void UCoreConditionProgress_Event::OnSatisfyChange_Implementation() {
    auto GameInstance = ProgressOwner->GetGameInstance<UCoreGameInstance>();
    auto EventSystem = GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
    EventSystem->RefreshEventHandler(this);
}