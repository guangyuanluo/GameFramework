// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "ChangeEffectEvent.generated.h"

class ACoreCharacter;
/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UChangeEffectEvent : public UGameEventBase
{
	GENERATED_BODY()

public:
    /**
    * 变化目标
    */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	ACoreCharacter* Character;
};
