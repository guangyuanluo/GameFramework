// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Modules/Assets/BackpackTypes.h"
#include "BackpackSetting.generated.h"

/*
* @brief 背包设置
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "背包设置"))
class GAMEFRAMEWORK_API UBackpackSetting : public UDeveloperSettings {
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override;
    /**
    * 物品优先级排序类
    */
    UPROPERTY(config, EditAnywhere, Category = "Backpack", meta = (DisplayName = "物品优先级排序类", MetaClass = "ItemSortPredicate", NoResetToDefault))
    FSoftClassPath ItemSortPredicateClass;

    /**
    * 玩家初始背包
    */
    UPROPERTY(config, EditAnywhere, Category = "Backpack", meta = (DisplayName = "玩家初始背包类型"))
    TArray<EBackpackTypeEnum> InitPackageTypes;

    /**
    * 背包扩展处理类
    */
    UPROPERTY(config, EditAnywhere, Category = "Backpack", meta = (DisplayName = "背包扩展处理类", MetaClass = "BackpackExtendHandler", NoResetToDefault))
    FSoftClassPath BackpackExtendHandlerClass;

    /**
    * 背包类型表
    */
    UPROPERTY(config, EditAnywhere, Category = "Backpack", meta = (DisplayName = "背包类型表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> BackpackTypeTable;
};