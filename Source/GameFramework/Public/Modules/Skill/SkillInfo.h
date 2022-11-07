// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "SkillInfo.generated.h"

USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FSkillInfo {
    GENERATED_BODY()

    const static int InvalidSkillId = -1;

    /** 技能等级 */
    UPROPERTY(Category = "Skill", EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "技能等级"))
    int SkillLevel = 1;

    /** 技能id */
    UPROPERTY(Category = "Skill", EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "技能id"))
    int SkillId = InvalidSkillId;
};

UCLASS()
class GAMEFRAMEWORK_API USkillInfoUtil : public UBlueprintFunctionLibrary {
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Skill")
    static bool IsValid(const FSkillInfo& SkillInfo);
};