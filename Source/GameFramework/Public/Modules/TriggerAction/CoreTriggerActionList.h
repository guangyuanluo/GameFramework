// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreTriggerActionList.generated.h"

/**
* 触发动作列表
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FCoreTriggerActionList {
    GENERATED_BODY()

    /** 触发动作列表 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "动作执行"))
    TArray<class UCoreTriggerAction*> Actions;
};