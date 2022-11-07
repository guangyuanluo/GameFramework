// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ScenarioSetting.generated.h"

/*
* @brief 剧情管理
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "剧情设置"))
class GAMEFRAMEWORK_API UScenarioSetting : public UDeveloperSettings {
    GENERATED_BODY()

public:
    virtual FName GetCategoryName() const override;
    /**
    * 播放剧情优先级排序类
    */
    UPROPERTY(config, EditAnywhere, Category = "剧情", meta = (DisplayName = "播放剧情优先级排序类", MetaClass = "PlayScenarioPredicate", NoResetToDefault))
    FSoftClassPath PlayScenarioPredicateClass;
};