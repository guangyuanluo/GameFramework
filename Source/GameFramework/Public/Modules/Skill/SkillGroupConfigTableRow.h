// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "Modules/Skill/SkillInfo.h"
#include "Modules/Skill/EffectInfo.h"
#include "SkillGroupConfigTableRow.generated.h"

/**
* 技能模板组表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FSkillGroupConfigTableRow : public FConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	* 技能模板组Id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "技能模板组Id", DisplayPriority = "1"))
	int32 SkillGroupID;

    /**
    * 技能模板组描述
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "技能模板组描述", DisplayPriority = "1"))
    FString Description;

    /** 技能类 */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "技能", DisplayPriority = "1"))
    TArray<FSkillInfo> Skills;

    /**
    * 效果类
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "效果", DisplayPriority = "1"))
    TArray<FEffectInfo> Effects;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
};