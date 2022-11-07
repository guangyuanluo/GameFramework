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

UAsyncListenEvents* UAsyncListenEvents::StartListen(UObject* WorldContextObject, const TArray<UClass*>& InListenEvents) {
    auto gameInstance = WorldContextObject->GetWorld()->GetGameInstance<UCoreGameInstance>();
    auto eventSystem = gameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
    UAsyncListenEvents* asyncListenEvents = NewObject<UAsyncListenEvents>(WorldContextObject);
    asyncListenEvents->ListenEvents = InListenEvents;
    eventSystem->RegistEventHandler(asyncListenEvents);
    asyncListenEvents->AddToRoot();

#if WITH_EDITORONLY_DATA
    asyncListenEvents->EndPlayDelegateHandle = FGameDelegates::Get().GetEndPlayMapDelegate().AddUObject(asyncListenEvents, &UAsyncListenEvents::OnMapChange, (uint32)0);
#endif

    return asyncListenEvents;
}

void UAsyncListenEvents::Abort() {
#if WITH_EDITORONLY_DATA
    FGameDelegates::Get().GetEndPlayMapDelegate().Remove(EndPlayDelegateHandle);
#endif
    auto gameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
    auto eventSystem = gameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
    eventSystem->UnRegistEventHandler(this);
    RemoveFromRoot();
}

TArray<UClass*> UAsyncListenEvents::GetHandleEventTypes_Implementation() {
    return ListenEvents;
}

void UAsyncListenEvents::OnEvent_Implementation(UCoreGameInstance* gameInstance, UGameEventBase* handleEvent) {
    OnEvent.Broadcast(gameInstance, handleEvent);
}

#if WITH_EDITORONLY_DATA
void UAsyncListenEvents::OnMapChange(uint32 MapChangeFlags) {
    RemoveFromRoot();
}
#endif