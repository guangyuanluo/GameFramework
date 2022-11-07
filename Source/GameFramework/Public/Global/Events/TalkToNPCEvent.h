// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "TalkToNPCEvent.generated.h"

/**
 * @brief 与NPC谈话事件
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UTalkToNPCEvent : public UGameEventBase
{
	GENERATED_BODY()

public:
	/**
	* 交互的来源entity
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	FString EntityId;

	/**
	* NPC id
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	int32 NPCID;
};