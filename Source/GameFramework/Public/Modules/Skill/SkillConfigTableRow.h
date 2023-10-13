// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "SkillConfigTableRow.generated.h"

/**
* 经验类型配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FSkillConfigTableRow : public FConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	* 技能Id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "技能Id", DisplayPriority = "1"))
	int32 SkillId;

    /**
    * 技能名字
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "技能名字", DisplayPriority = "1"))
    FString SkillName;

    /**
    * 技能描述
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "技能描述", DisplayPriority = "1"))
    FString Description;

    /**
    * 图标
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "技能图标", DisplayPriority = "1"))
    FString Icon;

    /**
    * 默认输入
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "默认输入", DisplayPriority = "1"))
    int32 InputID = INDEX_NONE;

    /** 技能类 */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill")
    TSubclassOf<class UGameplayAbility> GameplayAbilityClass;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
};