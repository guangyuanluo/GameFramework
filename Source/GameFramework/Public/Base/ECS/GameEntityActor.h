// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base/ECS/GameEntity.h"
#include "GameEntityActor.generated.h"

/**
 * 属于实体的Actor
 */
UCLASS()
class GAMEFRAMEWORK_API AGameEntityActor : public AActor, public IGameEntity
{
	GENERATED_BODY()
public:
	AGameEntityActor(const class FObjectInitializer& ObjectInitializer);

public:
	/** 覆写接口 */
	virtual FString& EntityID_Implementation() override;
	virtual void OnConstruction(const FTransform& Transform) override;
    virtual const FTransform& GetTransform() const override;
    virtual void SetTransform(const FTransform& NewTransform) override;

private:

	UPROPERTY(ReplicatedUsing = OnReq_EntityID)
	FString EntityID;

	UFUNCTION()
	void OnReq_EntityID(const FString& OldID);

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};