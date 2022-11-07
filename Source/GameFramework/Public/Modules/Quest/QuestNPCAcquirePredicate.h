// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestNPCAcquirePredicate.generated.h"

class UQuestComponent;
class UExecutingQuest;

/*
* @brief 任务抢占NPC优先级
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API UQuestNPCAcquirePredicate : public UObject {
    GENERATED_BODY()

public:
    /**
    * 任务抢占NPC优先级
    */
    UFUNCTION(BlueprintNativeEvent)
    bool Compare(UQuestComponent* QuestComponent, UExecutingQuest* A, UExecutingQuest* B);
};