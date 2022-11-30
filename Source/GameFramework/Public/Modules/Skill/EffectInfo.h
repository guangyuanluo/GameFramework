// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectInfo.generated.h"

USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FEffectInfo {
    GENERATED_BODY()

    /** 效果等级 */
    UPROPERTY(Category = "Skill", EditAnywhere, BlueprintReadOnly)
    int EffectLevel = 1;

    /** 效果类 */
	UPROPERTY(Category = "Skill", EditAnywhere, BlueprintReadOnly)
    int EffectID = -1;
};