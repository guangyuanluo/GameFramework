// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MoneySetting.generated.h"

/*
* @brief 货币设置
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "货币设置"))
class GAMEFRAMEWORK_API UMoneySetting : public UDeveloperSettings {
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override;
    /**
    * 货币类型表
    */
    UPROPERTY(config, EditAnywhere, Category = "Money", meta = (DisplayName = "货币类型表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> MoneyTypeTable;
};