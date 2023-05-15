// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActorDetectionComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsSettings.h"

//这里搞了一大段代码是因为引擎自带的胶囊体检测不带旋转的drawdebug，所以要把旋转透传进去应用到drawdebug
namespace ActtorDetection {
    const float KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE = 16.f;


    FCollisionQueryParams ConfigureCollisionParams(FName TraceTag, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf, const UObject* WorldContextObject) {
        FCollisionQueryParams Params(TraceTag, SCENE_QUERY_STAT_ONLY(KismetTraceUtils), bTraceComplex);
        Params.bReturnPhysicalMaterial = true;
        Params.bReturnFaceIndex = !UPhysicsSettings::Get()->bSuppressFaceRemapTable; // Ask for face index, as long as we didn't disable globally
        Params.AddIgnoredActors(ActorsToIgnore);
        if (bIgnoreSelf) {
            const AActor* IgnoreActor = Cast<AActor>(WorldContextObject);
            if (IgnoreActor) {
                Params.AddIgnoredActor(IgnoreActor);
            }
            else {
                // find owner
                const UObject* CurrentObject = WorldContextObject;
                while (CurrentObject) {
                    CurrentObject = CurrentObject->GetOuter();
                    IgnoreActor = Cast<AActor>(CurrentObject);
                    if (IgnoreActor) {
                        Params.AddIgnoredActor(IgnoreActor);
                        break;
                    }
                }
            }
        }

        return Params;
    }

#if ENABLE_DRAW_DEBUG
    void DrawDebugCapsuleTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Quat, float Radius, float HalfHeight, EDrawDebugTrace::Type DrawDebugType, bool bHit, const FHitResult& OutHit, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime) {
        if (DrawDebugType != EDrawDebugTrace::None) {
            bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
            float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

            if (bHit && OutHit.bBlockingHit) {
                // Red up to the blocking hit, green thereafter
                ::DrawDebugCapsule(World, Start, HalfHeight, Radius, Quat, TraceColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugCapsule(World, OutHit.Location, HalfHeight, Radius, Quat, TraceColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugLine(World, Start, OutHit.Location, TraceColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugPoint(World, OutHit.ImpactPoint, KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE, TraceColor.ToFColor(true), bPersistent, LifeTime);

                ::DrawDebugCapsule(World, End, HalfHeight, Radius, Quat, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugLine(World, OutHit.Location, End, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
            }
            else {
                // no hit means all red
                ::DrawDebugCapsule(World, Start, HalfHeight, Radius, Quat, TraceColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugCapsule(World, End, HalfHeight, Radius, Quat, TraceColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, LifeTime);
            }
        }
    }

    void DrawDebugCapsuleTraceMulti(const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Quat, float Radius, float HalfHeight, EDrawDebugTrace::Type DrawDebugType, bool bHit, const TArray<FHitResult>& OutHits, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime) {
        if (DrawDebugType != EDrawDebugTrace::None) {
            bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
            float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

            if (bHit && OutHits.Last().bBlockingHit) {
                // Red up to the blocking hit, green thereafter
                FVector const BlockingHitPoint = OutHits.Last().Location;
                ::DrawDebugCapsule(World, Start, HalfHeight, Radius, Quat, TraceColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugCapsule(World, BlockingHitPoint, HalfHeight, Radius, Quat, TraceColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugLine(World, Start, BlockingHitPoint, TraceColor.ToFColor(true), bPersistent, LifeTime);

                ::DrawDebugCapsule(World, End, HalfHeight, Radius, Quat, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugLine(World, BlockingHitPoint, End, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
            }
            else {
                // no hit means all red
                ::DrawDebugCapsule(World, Start, HalfHeight, Radius, Quat, TraceColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugCapsule(World, End, HalfHeight, Radius, Quat, TraceColor.ToFColor(true), bPersistent, LifeTime);
                ::DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, LifeTime);
            }

            // draw hits
            for (int32 HitIdx = 0; HitIdx < OutHits.Num(); ++HitIdx) {
                FHitResult const& Hit = OutHits[HitIdx];
                ::DrawDebugPoint(World, Hit.ImpactPoint, KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE, (Hit.bBlockingHit ? TraceColor.ToFColor(true) : TraceHitColor.ToFColor(true)), bPersistent, LifeTime);
            }
        }
    }
#endif  //#if ENABLE_DRAW_DEBUG

    bool CapsuleTraceMulti(const UObject* WorldContextObject, const FVector Start, const FVector End, const FQuat& Quat, float Radius, float HalfHeight, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime) {
        ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);

        static const FName CapsuleTraceMultiName(TEXT("CapsuleTraceMulti"));
        FCollisionQueryParams Params = ConfigureCollisionParams(CapsuleTraceMultiName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

        UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
        bool const bHit = World ? World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, CollisionChannel, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params) : false;

#if ENABLE_DRAW_DEBUG
        DrawDebugCapsuleTraceMulti(World, Start, End, Quat, Radius, HalfHeight, DrawDebugType, bHit, OutHits, TraceColor, TraceHitColor, DrawTime);
#endif

        return bHit;
    }


    bool CapsuleTraceSingle(const UObject* WorldContextObject, const FVector Start, const FVector End, const FQuat& Quat, float Radius, float HalfHeight, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime) {
        ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);

        static const FName CapsuleTraceSingleName(TEXT("CapsuleTraceSingle"));
        FCollisionQueryParams Params = ConfigureCollisionParams(CapsuleTraceSingleName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

        UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
        bool const bHit = World ? World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, CollisionChannel, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params) : false;

#if ENABLE_DRAW_DEBUG
        ActtorDetection::DrawDebugCapsuleTraceSingle(World, Start, End, Quat, Radius, HalfHeight, DrawDebugType, bHit, OutHit, TraceColor, TraceHitColor, DrawTime);
#endif

        return bHit;
    }
}

UActorDetectionComponent::UActorDetectionComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    bAutoActivate = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
    PrimaryComponentTick.bCanEverTick = true;
}

void UActorDetectionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UWorld* World = GetWorld();
    if (World == nullptr) {
        return;
    }

    AActor* Owner = GetOwner();
    if (Owner == nullptr) {
        return;
    }

    if (Owner->GetLocalRole() != ROLE_Authority) {
        return;
    }

    FTransform ComponentWorldTransform = GetComponentToWorld();
    FVector Start = ComponentWorldTransform.GetLocation();
    FRotator TraceRot = ComponentWorldTransform.GetRotation().Rotator();

    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(Owner);

    if (ExtraActorsToIgnore.Num() > 0) {
        for (auto ExtraActorToIgnore : ExtraActorsToIgnore) {
            if (ExtraActorToIgnore) {
                IgnoreActors.AddUnique(ExtraActorToIgnore);
            }
        }
    }

    FVector End = Start;
    if (DetectionWay == ActorDetectionWayEnum::E_Line) {
        End += TraceRot.Vector()* TraceLength;
    }

    //start和end相等，加微微偏移，不影响检测，两者完全相等似乎会导致impactpoint位置不对
    if (Start == End) {
        End = Start + FVector(0, 0, 0.0001f);
    }

    //最后修正
    if (bUseCCD) {
        if (LastTraceEndLocation != FVector::ZeroVector) {
            Start = LastTraceEndLocation;
        }
        LastTraceEndLocation = End;
    }

    TSet<AActor*> DetectedActorsCopy = DetectedActors;

    EDrawDebugTrace::Type DrawDebugTrace = (GIsEditor && bShowTrace) ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
    switch (DetectionWay) {
    case ActorDetectionWayEnum::E_Line:
    {
        //线条检测
        if (bTraceMulti) {
            TArray<FHitResult> HitResults;
            if (UKismetSystemLibrary::LineTraceMulti(this, Start, End, UEngineTypes::ConvertToTraceType(CollisionChannel), false, IgnoreActors, DrawDebugTrace, HitResults, true)) {
                for (auto& HitResult : HitResults) {
                    HandleHitResult(HitResult, DetectedActorsCopy);
                }
            }
            else {
                FHitResult HitResult;
                if (UKismetSystemLibrary::LineTraceSingle(this, Start, End, UEngineTypes::ConvertToTraceType(CollisionChannel), false, IgnoreActors, DrawDebugTrace, HitResult, true)) {
                    HandleHitResult(HitResult, DetectedActorsCopy);
                }
            }
        }
    }
    break;
    case ActorDetectionWayEnum::E_Sphere:
        //球型检测
        if (bTraceMulti) {
            TArray<FHitResult> HitResults;
            if (UKismetSystemLibrary::SphereTraceMulti(this, Start, End, SphereRadius, UEngineTypes::ConvertToTraceType(CollisionChannel), false, IgnoreActors, DrawDebugTrace, HitResults, true, FLinearColor::Green, FLinearColor::Red, -1.f)) {
                for (auto& HitResult : HitResults) {
                    HandleHitResult(HitResult, DetectedActorsCopy);
                }
            }
        }
        else {
            FHitResult HitResult;
            if (UKismetSystemLibrary::SphereTraceSingle(this, Start, End, SphereRadius, UEngineTypes::ConvertToTraceType(CollisionChannel), false, IgnoreActors, DrawDebugTrace, HitResult, true, FLinearColor::Green, FLinearColor::Red, -1.f)) {
                HandleHitResult(HitResult, DetectedActorsCopy);
            }
        }
    break;
    case ActorDetectionWayEnum::E_Capsule:
        //胶囊体检测
        if (bTraceMulti) {
            TArray<FHitResult> HitResults;
            if (ActtorDetection::CapsuleTraceMulti(this, Start, End, ComponentWorldTransform.GetRotation(), CapsuleRadius, CapsuleHalfHeight, UEngineTypes::ConvertToTraceType(CollisionChannel), false, IgnoreActors, DrawDebugTrace, HitResults, true, FLinearColor::Green, FLinearColor::Red, -1.f)) {
                for (auto& HitResult : HitResults) {
                    HandleHitResult(HitResult, DetectedActorsCopy);
                }
            }
        }
        else {
            FHitResult HitResult;
            if (ActtorDetection::CapsuleTraceSingle(this, Start, End, ComponentWorldTransform.GetRotation(), CapsuleRadius, CapsuleHalfHeight, UEngineTypes::ConvertToTraceType(CollisionChannel), false, IgnoreActors, DrawDebugTrace, HitResult, true, FLinearColor::Green, FLinearColor::Red, -1.f)) {
                HandleHitResult(HitResult, DetectedActorsCopy);
            }
        }
    break;
    }
    
    //剩下没移除的，说明已经不在hit中了
    for (auto Actor : DetectedActorsCopy) {
        DetectedActors.Remove(Actor);

        //广播
        OnDetectExitActor.Broadcast(Actor);
    }
}

bool UActorDetectionComponent::IsValidHitClass(UClass* HitClass) {
    if (ClassesToConsider.Num() == 0) {
        return true;
    }

    for (int i = 0; i < ClassesToConsider.Num(); i++) {
        if (HitClass->IsChildOf(ClassesToConsider[i])) {
            return true;
        }
    }

    return false;
}

void UActorDetectionComponent::HandleHitResult(const FHitResult& HitResult, TSet<AActor*>& DetectedActorsCopy) {
    if (HitResult.HasValidHitObjectHandle()) {
        AActor* HitActor = HitResult.GetActor();

        if (IsValidHitClass(HitActor->GetClass())) {
            if (!DetectedActors.Contains(HitActor)) {
                DetectedActors.Add(HitActor);

                // Broadcast the hit
                OnDetectActor.Broadcast(HitActor, HitResult);
            }
            else {
                DetectedActorsCopy.Remove(HitActor);
            }
        }
    }
}