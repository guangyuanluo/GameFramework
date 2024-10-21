// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "Modules/Skill/SkillInfo.h"
#include "Modules/Skill/EffectInfo.h"
#include "Modules/Item/ItemTypes.h"
#include "Modules/Skill/SkillGroupIDContainer.h"
#include "ItemConfigTableRow.generated.h"

/**
* 物品配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FItemConfigTableRow : public FConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	* 物品id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "物品Id", DisplayPriority = "1"))
	int32 ItemId;

	/**
	* 物品类型
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable|Hide", meta = (DisplayName = "物品类型", DisplayPriority = "1"))
	EItemTypeEnum ItemType;

	/**
	* 物品图标
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "物品图标", DisplayPriority = "1"))
	TSoftObjectPtr<class UTexture2D> ItemIcon;

	/**
	* 物品mesh
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "物品Mesh", DisplayPriority = "1"))
	TSoftObjectPtr<class UStaticMesh> ItemMesh;

	/**
	* 物品蓝图
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "物品蓝图类", DisplayPriority = "1"))
	TSubclassOf<class UCoreItem> ItemClass;

	/**
	* 物品名字
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "物品名字", DisplayPriority = "1"))
	FString ItemName;

	/**
	* 物品描述
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "物品描述", DisplayPriority = "1"))
	FString ItemDescription;

	/**
	* 自动使用
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "自动使用", DisplayPriority = "1"))
	bool AutoUse = false;

	/**
	* 死亡掉落
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "死亡掉落", DisplayPriority = "1"))
	bool DropWhenDead = false;

	/**
	* 可被丢弃
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "可被丢弃", DisplayPriority = "1"))
	bool Discardable = true;

	/**
	* 是否消耗品
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "是否消耗品", DisplayPriority = "1"))
	bool BeConsumed = true;

	/**
	* 堆叠上限，为0表示无限制
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "堆叠上限", DisplayPriority = "1"))
	int MaxStack = 0;

	/**
	* 拥有上限，为0表示无限制
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemConfigTable", meta = (DisplayName = "拥有上限", DisplayPriority = "1"))
	int MaxOwner = 0;

	/**
	* 初始技能模组
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "技能", meta = (DisplayName = "技能模组", DisplayPriority = "1"))
	FSkillGroupIDContainer SkillGroupIDContainer;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
};