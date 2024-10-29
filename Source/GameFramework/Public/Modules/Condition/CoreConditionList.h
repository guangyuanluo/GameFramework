// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreConditionList.generated.h"

USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FCoreConditionList {
    GENERATED_BODY()

    /** 条件列表 */
    UPROPERTY(Category = "Conditon", EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "条件列表"))
    TArray<TObjectPtr<class UCoreCondition>> Conditions;
};