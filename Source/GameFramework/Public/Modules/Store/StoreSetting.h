// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "StoreSetting.generated.h"

/*
* @brief 商店设置
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "商店设置"))
class GAMEFRAMEWORK_API UStoreSetting : public UDeveloperSettings {
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override;
    /**
    * 商品表
    */
    UPROPERTY(config, EditAnywhere, Category = "Store", meta = (DisplayName = "商品表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> GoodsTable;

    /**
    * 商店表
    */
    UPROPERTY(config, EditAnywhere, Category = "Store", meta = (DisplayName = "商店表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> StoreTable;
};