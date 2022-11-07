// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "EnterAreaEvent.generated.h"

class ACoreCharacter;

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UEnterAreaEvent : public UGameEventBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	ACoreCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	FString AreaName;
};