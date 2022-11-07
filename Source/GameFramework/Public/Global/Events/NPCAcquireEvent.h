// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "NPCAcquireEvent.generated.h"

/**
 * @brief NPC占用事件
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UNPCAcquireEvent : public UGameEventBase
{
	GENERATED_BODY()

public:
	/**
	* 谁占有了
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	class AActor* Source;

	/**
	* 占用NPC
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	TArray<int32> NPCIDs;

	/**
	* 占有时传入的自定义数据
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	UObject* CustomInfo;
};