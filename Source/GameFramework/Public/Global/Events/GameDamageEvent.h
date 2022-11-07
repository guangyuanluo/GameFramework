// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "GameDamageEvent.generated.h"

class ACoreCharacter;
/**
 * @brief 造成伤害事件
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UGameDamageEvent : public UGameEventBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	ACoreCharacter* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	ACoreCharacter* Source;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	int32 DamageType;
};