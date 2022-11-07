// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "CompleteQuestEvent.generated.h"

/**
 * @brief 完成任务event
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UCompleteQuestEvent : public UGameEventBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	class AActor* Source;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	int32 CompleteQuestId;
};