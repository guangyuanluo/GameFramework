// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "CurrentComboSectionLimitCondition.generated.h"

UCLASS(meta = (DisplayName = "前置连招检查", ToolTip = "前置连招检查", ShowDisplayNames, Category = "技能条件"))
class GAMEFRAMEWORK_API UCurrentComboSectionLimitCondition : public UCoreCondition {
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SectionName;
};

UCLASS()
class GAMEFRAMEWORK_API UCurrentComboSectionLimitConditionProgress : public UCoreConditionProgress {
    GENERATED_BODY()

public:
    virtual bool IsComplete_Implementation(bool& IsValid) override;
};