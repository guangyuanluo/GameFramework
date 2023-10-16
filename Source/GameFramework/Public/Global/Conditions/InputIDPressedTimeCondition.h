// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "InputIDPressedTimeCondition.generated.h"

UCLASS(meta = (DisplayName = "按键按下时间", ShowDisplayNames, Category = "通用条件"))
class GAMEFRAMEWORK_API UInputIDPressedTimeCondition : public UCoreCondition {
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int32 InputID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float InputTimeLimit;
};

UCLASS()
class GAMEFRAMEWORK_API UInputIDPressedTimeConditionProgress : public UCoreConditionProgress {
    GENERATED_BODY()

public:
    virtual bool IsComplete(bool& IsValid) override;
};