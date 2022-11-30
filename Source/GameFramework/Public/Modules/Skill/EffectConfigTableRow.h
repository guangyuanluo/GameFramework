// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "EffectConfigTableRow.generated.h"

/**
* 效果配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FEffectConfigTableRow : public FConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	* 效果Id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "效果Id", DisplayPriority = "1"))
	int32 EffectID;

    /**
    * 效果名字
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "名字", DisplayPriority = "1"))
    FString EffectName;

    /**
    * 效果描述
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "描述", DisplayPriority = "1"))
    FString Description;

    /**
    * 图标
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill", meta = (DisplayName = "图标", DisplayPriority = "1"))
    FString Icon;

    /** 效果类 */
    UPROPERTY(Category = "Skill", EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class UGameplayEffect> GameplayEffectClass;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
};