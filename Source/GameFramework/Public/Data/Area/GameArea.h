// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/GameEntityActor.h"
#include "GameArea.generated.h"

class ACoreCharacter;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, abstract)
class GAMEFRAMEWORK_API AGameArea : public AGameEntityActor
{
	GENERATED_BODY()
	
public:
	AGameArea(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	FString AreaName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	TArray<AGameArea*> LinkArea;

	UFUNCTION(BlueprintCallable, Category = "Logic")
	void OnEnter(ACoreCharacter* character);

	UFUNCTION(BlueprintCallable, Category = "Logic")
	void OnExit(ACoreCharacter* character);
};