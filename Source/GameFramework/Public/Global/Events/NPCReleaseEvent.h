// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "NPCReleaseEvent.generated.h"

/**
 * @brief 释放NPC占用事件
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UNPCReleaseEvent : public UGameEventBase
{
	GENERATED_BODY()

public:
	/**
	* 谁释放了
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	class AActor* Source;

	/**
	* 释放NPC
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	TArray<int32> NPCIDs;
};