// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "Modules/Exp/ExpTypes.h"
#include "ExpLevelUpEvent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UExpLevelUpEvent : public UGameEventBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	class AActor* Source;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	EExpTypeEnum ExpType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
    int ExpLevel;
};