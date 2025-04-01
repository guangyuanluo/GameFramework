// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEventUtils.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "EventSystem.h"

void UGameEventUtils::PushEvent(UObject* WorldContextObject, class UGameEventBase* InPushEvent) {
    if (!WorldContextObject) {
        return;
    }
    auto World = WorldContextObject->GetWorld();
    if (!World) {
        return;
    }
    auto GameInstance = World->GetGameInstance<UCoreGameInstance>();
    if (!GameInstance) {
        return;
    }
    auto EventSystem = GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
    if (!EventSystem) {
        return;
    }
    EventSystem->PushEvent(InPushEvent);
}

void UGameEventUtils::PushEventToServer(UObject* WorldContextObject, class UGameEventBase* InPushEvent, bool Reliable) {
    if (!WorldContextObject) {
        return;
    }
    auto World = WorldContextObject->GetWorld();
    if (!World) {
        return;
    }
    auto GameInstance = World->GetGameInstance<UCoreGameInstance>();
    if (!GameInstance) {
        return;
    }
    auto EventSystem = GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
    if (!EventSystem) {
        return;
    }
    EventSystem->PushEventToServer(InPushEvent, Reliable);
}

void UGameEventUtils::PushEventToClient(UObject* WorldContextObject, class ACorePlayerController* PlayerController, class UGameEventBase* InPushEvent) {
    if (!WorldContextObject) {
        return;
    }
    auto World = WorldContextObject->GetWorld();
    if (!World) {
        return;
    }
    auto GameInstance = World->GetGameInstance<UCoreGameInstance>();
    if (!GameInstance) {
        return;
    }
    auto EventSystem = GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
    if (!EventSystem) {
        return;
    }
    EventSystem->PushEventToClient(PlayerController, InPushEvent);
}