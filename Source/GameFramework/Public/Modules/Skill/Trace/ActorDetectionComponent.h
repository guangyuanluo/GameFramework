// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/PrimitiveComponent.h"
#include "WorldCollision.h"

#include "ActorDetectionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDetectActor, AActor*, DetectedActor, const FHitResult&, HitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDetectExitActor, AActor*, DetectedActor);

UENUM(BlueprintType)
enum class ActorDetectionWayEnum :uint8 {
    E_Line UMETA(DisplayName = "线条检测"),
    E_Sphere UMETA(DisplayName = "球型检测"),
    E_Capsule UMETA(DisplayName = "胶囊体检测"),
};

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class GAMEFRAMEWORK_API UActorDetectionComponent : public UPrimitiveComponent {
    GENERATED_BODY()

public:

    UActorDetectionComponent(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintAssignable)
    FOnDetectActor OnDetectActor;

    UPROPERTY(BlueprintAssignable)
    FOnDetectExitActor OnDetectExitActor;

    UPROPERTY(EditAnywhere)
    TArray<UClass*> ClassesToConsider;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Pawn;

    UPROPERTY(EditAnywhere)
    ActorDetectionWayEnum DetectionWay;

    UPROPERTY(EditAnywhere, meta = (EditCondition = "DetectionWay == ActorDetectionWayEnum::E_Line"))
    float TraceLength = 0.0f;

    UPROPERTY(EditAnywhere, meta = (EditCondition = "DetectionWay == ActorDetectionWayEnum::E_Sphere"))
    float SphereRadius = 0.0f;

    UPROPERTY(EditAnywhere, meta = (EditCondition = "DetectionWay == ActorDetectionWayEnum::E_Capsule"))
    float CapsuleRadius = 0.0f;

    UPROPERTY(EditAnywhere, meta = (EditCondition = "DetectionWay == ActorDetectionWayEnum::E_Capsule"))
    float CapsuleHalfHeight = 0.0f;

    UPROPERTY(Transient)
    TSet<AActor*> DetectedActors;

    UPROPERTY(Transient)
    FVector LastTraceEndLocation = FVector::ZeroVector;

    /**
     * 是否显示射线
     */
    UPROPERTY(EditAnywhere)
    bool bShowTrace = false;

    /**
     * 是否打开ccd（连续碰撞检测），不打开情况下，以组件当前位置做扫描，某些速度太快的场景，比如子弹，要扫描弹道中命中的目标，就要连续检测，需要打开此开关
     */
    UPROPERTY(EditAnywhere)
    bool bUseCCD = false;

    /**
     * 是否trace多个
     */
    UPROPERTY(EditAnywhere)
    bool bTraceMulti = false;

    /**
    * 额外要忽略检测的actor
    */
    UPROPERTY(EditAnywhere)
    TArray<TObjectPtr<AActor>> ExtraActorsToIgnore;

    /** Color used to draw the shape. */
    UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = Shape)
    FColor ShapeColor;

    /** Only show this component if the actor is selected */
    UPROPERTY()
    uint8 bDrawOnlyIfSelected : 1;

    bool IsValidHitClass(UClass* HitClass);

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    //~ Begin UPrimitiveComponent Interface.
    virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
    //~ End UPrimitiveComponent Interface.
private:
    void HandleHitResult(const FHitResult& HitResult, TSet<AActor*>& DetectedActorsCopy);
};