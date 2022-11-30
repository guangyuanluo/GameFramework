// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillInfo.generated.h"

USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FSkillInfo {
    GENERATED_BODY()

    /** 技能等级 */
    UPROPERTY(Category = "Skill", EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "技能等级"))
    int SkillLevel = 1;

    /** 技能类 */
    UPROPERTY(Category = "Skill", EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "技能类"))
    int SkillID = -1;
};