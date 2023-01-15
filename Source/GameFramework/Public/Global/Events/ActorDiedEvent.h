// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "ActorDiedEvent.generated.h"

class ACoreCharacter;
/**
 * @brief 击杀master event
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UActorDiedEvent : public UGameEventBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	ACoreCharacter* DiedUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	AActor* KillUnit;
};