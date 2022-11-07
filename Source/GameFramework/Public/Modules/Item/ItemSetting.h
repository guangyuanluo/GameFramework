// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ItemSetting.generated.h"

/*
* @brief 物品设置
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "物品设置"))
class GAMEFRAMEWORK_API UItemSetting : public UDeveloperSettings {
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override;

    /**
    * 物品类型表
    */
    UPROPERTY(config, EditAnywhere, Category = "Exp", meta = (DisplayName = "物品类型表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> ItemTypeTable;

    /**
    * 物品表
    */
    UPROPERTY(config, EditAnywhere, Category = "Exp", meta = (DisplayName = "物品表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> ItemTable;
};