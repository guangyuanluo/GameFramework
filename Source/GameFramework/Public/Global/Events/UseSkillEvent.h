// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "UseSkillEvent.generated.h"

class ACoreCharacter;
/**
 * @brief 使用技能 event
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UUseSkillEvent : public UGameEventBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	ACoreCharacter* Unit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	int32 Skill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	TArray<ACoreCharacter*> Targets;
};