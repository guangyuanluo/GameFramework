// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "GameplayEffectTypes.h"
#include "ChangeAttributeEvent.generated.h"

class ACoreCharacterStateBase;
/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UChangeAttributeEvent : public UGameEventBase
{
	GENERATED_BODY()

public:
    /**
    * 变化目标
    */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
    ACoreCharacterStateBase* OwnerActor;

    /**
    * 变化属性
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
    FGameplayAttribute Attribute;

    /**
    * 老数值属性
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
    float OldValue;

    /**
    * 新数值属性
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
    float NewValue;
};
