// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayScenarioPredicate.generated.h"

/*
* @brief 播放剧情优先级
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API UPlayScenarioPredicate : public UObject {
    GENERATED_BODY()

public:
    /**
    * 播放剧情优先级
    */
    UFUNCTION(BlueprintNativeEvent)
    bool Compare(class UAsyncPlayScenario* A, class UAsyncPlayScenario* B) const;
};