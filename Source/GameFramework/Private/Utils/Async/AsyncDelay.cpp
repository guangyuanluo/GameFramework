// Copyright Epic Games, Inc. All Rights Reserved.

#include "AsyncDelay.h"
#include "Engine/World.h"
#include "TimerManager.h"

UAsyncDelay::UAsyncDelay(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    
}

UAsyncDelay* UAsyncDelay::DelayExecute(UObject* WorldContextObject, float DelayTime) {
    UAsyncDelay* AsyncDelay = NewObject<UAsyncDelay>(WorldContextObject);
    AsyncDelay->Start(DelayTime);
    AsyncDelay->AddToRoot();

    return AsyncDelay;
}

void UAsyncDelay::Start(float DelayTime) {
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAsyncDelay::TimerCallback, DelayTime, false);
}

void UAsyncDelay::TimerCallback() {
    OnComplete.Broadcast();
    RemoveFromRoot();
}