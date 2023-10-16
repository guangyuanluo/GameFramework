// Copyright Epic Games, Inc. All Rights Reserved.

#include "AsyncListenEvents.h"
#include "CoreGameInstance.h"
#include "Engine/World.h"
#include "GameSystemManager.h"
#include "EventSystem.h"

#if WITH_EDITORONLY_DATA
#include "GameDelegates.h"
#endif

UAsyncListenEvents::UAsyncListenEvents(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {

}

UAsyncListenEvents* UAsyncListenEvents::StartListen(UObject* WorldContextObject, const TArray<TSubclassOf<class UGameEventBase>>& InListenEvents) {
    auto gameInstance = WorldContextObject->GetWorld()->GetGameInstance<UCoreGameInstance>();
    auto eventSystem = gameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
    UAsyncListenEvents* AsyncListenEvents = NewObject<UAsyncListenEvents>(WorldContextObject);
    AsyncListenEvents->ListenEvents = InListenEvents;
    eventSystem->RegistEventHandler(AsyncListenEvents);
    AsyncListenEvents->AddToRoot();

#if WITH_EDITORONLY_DATA
    AsyncListenEvents->EndPlayDelegateHandle = FGameDelegates::Get().GetEndPlayMapDelegate().AddUObject(AsyncListenEvents, &UAsyncListenEvents::OnMapChange, (uint32)0);
#endif

    return AsyncListenEvents;
}

void UAsyncListenEvents::Abort() {
#if WITH_EDITORONLY_DATA
    FGameDelegates::Get().GetEndPlayMapDelegate().Remove(EndPlayDelegateHandle);
#endif
    auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
    auto EventSystem = GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
    EventSystem->UnRegistEventHandler(this);
    RemoveFromRoot();
}

TArray<TSubclassOf<class UGameEventBase>> UAsyncListenEvents::GetHandleEventTypes() {
    return ListenEvents;
}

void UAsyncListenEvents::OnEvent(UCoreGameInstance* gameInstance, UGameEventBase* handleEvent) {
    OnEventCallback.Broadcast(gameInstance, handleEvent);
}

#if WITH_EDITORONLY_DATA
void UAsyncListenEvents::OnMapChange(uint32 MapChangeFlags) {
    RemoveFromRoot();
}
#endif