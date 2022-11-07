// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ExpSetting.generated.h"

/*
* @brief 经验设置
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "经验设置"))
class GAMEFRAMEWORK_API UExpSetting : public UDeveloperSettings {
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override;
    /**
    * 经验类型表
    */
    UPROPERTY(config, EditAnywhere, Category = "Exp", meta = (DisplayName = "经验类型表", NoResetToDefault))
    TSoftObjectPtr<class UDataTable> ExpTypeTable;
};