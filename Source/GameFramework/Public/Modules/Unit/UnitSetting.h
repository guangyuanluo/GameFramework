// Fill out your copyright notice in the Description Page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UnitSetting.generated.h"

/*
* @brief 单位设置
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "单位设置"))
class GAMEFRAMEWORK_API UUnitSetting : public UDeveloperSettings {
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override;

    /**
    * 单位表
    */
    UPROPERTY(config, EditAnywhere, Category = "Exp", meta = (DisplayName = "单位表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> UnitTable;

    /**
    * 单位组表
    */
    UPROPERTY(config, EditAnywhere, Category = "Exp", meta = (DisplayName = "单位组表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> UnitGroupTable;
};