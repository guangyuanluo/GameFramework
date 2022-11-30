// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "Modules/Skill/SkillInfo.h"
#include "Modules/Skill/EffectInfo.h"
#include "UnitInfoConfigTableRow.generated.h"

/**
* 单位配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FUnitInfoConfigTableRow : public FConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	*  单位Id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "基础", meta = (DisplayName = "单位Id", DisplayPriority = "1"))
	int32 UnitId;

	/**
	*  单位名字
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "基础", meta = (DisplayName = "单位名字", DisplayPriority = "1"))
	FString UnitName;

	/**
	*  单位描述
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "基础", meta = (DisplayName = "单位描述", DisplayPriority = "1"))
	FString Description;

	/**
	*  单位2D头像
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "基础", meta = (DisplayName = "单位2D头像", DisplayPriority = "1"))
	FString HeadIcon;

    /**
	*  单位海报图片
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "基础", meta = (DisplayName = "单位海报图片", DisplayPriority = "1"))
	FString PosterImage;

	/**
	*  单位蓝图
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "基础", meta = (DisplayName = "单位蓝图", DisplayPriority = "1"))
	TSoftClassPtr<class ACoreCharacter> CharacterClass;

	/**
	* 单位等级
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "属性", meta = (DisplayName = "单位等级", DisplayPriority = "1"))
	int Level = 1;

	/**
	*  成长经验Id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UnitSystem|Hide", meta = (DisplayName = "成长经验Id", DisplayPriority = "1"))
	int GrowExpTypeId = -1;

    /**
    * 使用属性集
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性", meta = (DisplayName = "使用属性集", DisplayPriority = "1"))
    TSubclassOf<class UAttributeSet > AttributeSetClass;

	/**
	* 属性效果类
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性", meta = (DisplayName = "属性效果类", DisplayPriority = "1"))
	TSubclassOf<class UGameplayEffect> AttributeEffectClass;

	/**
	* 属性表
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "属性", meta = (DisplayName = "属性表", DisplayPriority = "1"))
	TSoftObjectPtr<class UCurveTable> AttributeTable;

	/**
	* 初始技能模组
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能|Hide", meta = (DisplayName = "技能模组id", DisplayPriority = "1"))
	int SkillGroupID = -1;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
};