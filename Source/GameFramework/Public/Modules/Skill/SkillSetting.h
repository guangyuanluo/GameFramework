// Fill out your copyright notice in the Description Page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "SkillSetting.generated.h"

/*
* @brief 技能设置
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "技能设置"))
class GAMEFRAMEWORK_API USkillSetting : public UDeveloperSettings {
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override;

    /**
    * 技能表
    */
    UPROPERTY(config, EditAnywhere, Category = "Skill", meta = (DisplayName = "技能表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> SkillTable;

    /**
    * 效果表
    */
    UPROPERTY(config, EditAnywhere, Category = "Skill", meta = (DisplayName = "效果表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> EffectTable;

    /**
    * 技能模组
    */
    UPROPERTY(config, EditAnywhere, Category = "Skill", meta = (DisplayName = "技能模组表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> SkillGroupTable;

    /**
    * 全局连招条件检查
    */
    UPROPERTY(config, EditAnywhere, Category = "Skill", meta = (DisplayName = "全局连招条件检查", NoResetToDefault))
    TArray<TSubclassOf<class UCoreAbilityCondition>> GlobalComboConditions;

    /**
    * 连招技能tag
    */
    UPROPERTY(config, EditAnywhere, Category = "Skill", meta = (DisplayName = "技能模组表", NoResetToDefault))
    FGameplayTag ComboSkillTag;
};