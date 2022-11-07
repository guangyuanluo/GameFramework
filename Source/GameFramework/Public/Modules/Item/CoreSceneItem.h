// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayAbilitySpec.h"
#include "Base/ECS/GameEntityActor.h"
#include "CoreSceneItem.generated.h"

/**
 * 场景物品
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API ACoreSceneItem : public AGameEntityActor
{
	GENERATED_BODY()
	
public:
    ACoreSceneItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	class UItemComponent* ItemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMeshComponent;

	UFUNCTION(BlueprintCallable)
	void UpdateItem(int ItemId, int ItemCount);

	virtual void OnConstruction(const FTransform& Transform) override;
};