﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "ExitAreaEvent.generated.h"

class ACoreCharacter;

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UExitAreaEvent : public UGameEventBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	ACoreCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	FString AreaName;
};