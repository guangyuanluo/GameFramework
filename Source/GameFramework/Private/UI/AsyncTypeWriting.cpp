// Copyright Epic Games, Inc. All Rights Reserved.

#include "AsyncTypeWriting.h"
#include "Engine/World.h"
#include "TimerManager.h"

#if WITH_EDITORONLY_DATA
#include "GameDelegates.h"
#endif

UAsyncTypeWriting::UAsyncTypeWriting(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {

}

UAsyncTypeWriting* UAsyncTypeWriting::StartTypeWriting(UObject* WorldContextObject, const FString& Text, float PerCharSecond) {
    UAsyncTypeWriting* AsyncTypeWriting = NewObject<UAsyncTypeWriting>(WorldContextObject);
    AsyncTypeWriting->Start(Text, PerCharSecond);
    AsyncTypeWriting->AddToRoot();

    return AsyncTypeWriting;
}

void UAsyncTypeWriting::Start(const FString& InText, float InPerCharSecond) {
    this->Text = InText;
    this->PerCharSecond = InPerCharSecond;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAsyncTypeWriting::TimerCallback, PerCharSecond, true);
#if WITH_EDITORONLY_DATA
    EndPlayDelegateHandle = FGameDelegates::Get().GetEndPlayMapDelegate().AddUObject(this, &UAsyncTypeWriting::OnMapChange, (uint32)0);
#endif
}

void UAsyncTypeWriting::TimerCallback() {
    if (NowText.Len() == Text.Len()) {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        OnComplete.Broadcast(Text);

#if WITH_EDITORONLY_DATA
        FGameDelegates::Get().GetEndPlayMapDelegate().Remove(EndPlayDelegateHandle);
#endif
        RemoveFromRoot();
    }
    else {
        NowText.AppendChar(Text[NowText.Len()]);
        OnTextChange.Broadcast(NowText);
    }
}

#if WITH_EDITORONLY_DATA
void UAsyncTypeWriting::OnMapChange(uint32 MapChangeFlags) {
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    RemoveFromRoot();
}
#endif