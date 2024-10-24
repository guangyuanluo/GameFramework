// Fill out your copyright notice in the Description Page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "QuestSetting.generated.h"

/*
* @brief 任务设置
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "任务设置"))
class GAMEFRAMEWORK_API UQuestSetting : public UDeveloperSettings {
    GENERATED_BODY()

public:
    /**
    * 预加载任务树
    */
    UPROPERTY(config, EditAnywhere, Category = "任务", meta = (DisplayName = "预加载任务树", NoResetToDefault))
    TArray<TSoftObjectPtr<class UQuestTree>> PreloadQuestTrees;

    virtual FName GetCategoryName() const override;
};