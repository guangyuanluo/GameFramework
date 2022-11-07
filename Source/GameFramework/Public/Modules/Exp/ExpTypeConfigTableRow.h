// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "ExpTypeConfigTableRow.generated.h"

/**
* 经验类型配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FExpTypeConfigTableRow : public FConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	static const int RoleExpType = 0;			///< 人物成长经验类型

	/**
	* 经验类型Id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ExpTypeConfigTable", meta = (DisplayName = "经验类型Id", DisplayPriority = "1"))
	int32 ExpTypeId;

	/**
	* 经验类型描述
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ExpTypeConfigTable", meta = (DisplayName = "经验类型描述", DisplayPriority = "1"))
	FString ExpTypeDescription;

	/**
	* 经验icon
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ExpTypeConfigTable", meta = (DisplayName = "经验类型图标", DisplayPriority = "1"))
	FString Icon;

	/**
	* 经验等级表
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ExpTypeConfigTable", meta = (DisplayName = "经验升级表", DisplayPriority = "1"))
	TSoftObjectPtr<class UCurveTable> ExpLevelTable;

    virtual int GetUniqueId() override;
    virtual FString GetSimpleDescription() override;
};