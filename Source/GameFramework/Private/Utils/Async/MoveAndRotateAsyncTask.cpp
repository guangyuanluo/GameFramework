// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoveAndRotateAsyncTask.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UMoveAndRotateAsyncTask::UMoveAndRotateAsyncTask(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    
}

UMoveAndRotateAsyncTask* UMoveAndRotateAsyncTask::MoveAndRotateAsync(class AActor* InActor, float InTransitionTime, const FVector& InTargetLocation, float InTargetYaw) {
    UMoveAndRotateAsyncTask* MoveAndRotateAsyncTask = NewObject<UMoveAndRotateAsyncTask>(InActor);
    MoveAndRotateAsyncTask->Start(InActor, InTransitionTime, InTargetLocation, InTargetYaw);
    MoveAndRotateAsyncTask->AddToRoot();

    return MoveAndRotateAsyncTask;
}

void UMoveAndRotateAsyncTask::Start(class AActor* InActor, float InTransitionTime, const FVector& InTargetLocation, float InTargetYaw) {
    Actor = InActor;
    TransitionTime = InTransitionTime;
    StartLocation = Actor->GetActorLocation();
    StartYaw = Actor->GetActorRotation().Yaw;
    TargetLocation = InTargetLocation;
    TargetYaw = InTargetYaw;
    TimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UMoveAndRotateAsyncTask::TimerCallback);
}

void UMoveAndRotateAsyncTask::TimerCallback() {
    auto World = Actor->GetWorld();
    auto DeltaTime = World->DeltaTimeSeconds;
    PlayTime += DeltaTime;
    if (PlayTime >= TransitionTime) {
        Actor->SetActorLocation(TargetLocation);
        auto Rotation = Actor->GetActorRotation();
        Rotation.Yaw = TargetYaw;
        Actor->SetActorRotation(Rotation);

        World->GetTimerManager().ClearTimer(TimerHandle);

        OnComplete.Broadcast();
        RemoveFromRoot();
    }
    else {
        auto LerpValue = PlayTime / TransitionTime;
        FVector LerpLocation = UKismetMathLibrary::VLerp(StartLocation, TargetLocation, LerpValue);
        Actor->SetActorLocation(LerpLocation);
        float LerpYaw = UKismetMathLibrary::Lerp(StartYaw, TargetYaw, LerpValue);
        auto Rotation = Actor->GetActorRotation();
        Rotation.Yaw = LerpYaw;
        Actor->SetActorRotation(Rotation);

        TimerHandle = World->GetTimerManager().SetTimerForNextTick(this, &UMoveAndRotateAsyncTask::TimerCallback);
    }
}