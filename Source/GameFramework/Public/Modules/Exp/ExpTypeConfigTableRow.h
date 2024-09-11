// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "Modules/Exp/ExpTypes.h"
#include "ExpTypeConfigTableRow.generated.h"

/**
* 经验类型配置表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FExpTypeConfigTableRow : public FConfigTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/**
	* 经验类型Id
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ExpTypeConfigTable", meta = (DisplayName = "经验类型Id", DisplayPriority = "1"))
	int32 ExpTypeId;

	/**
	* 经验类型枚举
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ExpTypeConfigTable", meta = (DisplayName = "经验类型Id", DisplayPriority = "1"))
	EExpTypeEnum ExpType;

	/**
	* 经验icon
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ExpTypeConfigTable", meta = (DisplayName = "经验类型图标", DisplayPriority = "1"))
	TSoftObjectPtr<class UTexture2D> Icon;

	/**
	* 经验等级表
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ExpTypeConfigTable", meta = (DisplayName = "经验升级表", DisplayPriority = "1"))
	TSoftObjectPtr<class UCurveTable> ExpLevelTable;

    virtual int GetUniqueId() override;
	virtual int GetRowUniqueId() override;
    virtual FString GetSimpleDescription() override;
};