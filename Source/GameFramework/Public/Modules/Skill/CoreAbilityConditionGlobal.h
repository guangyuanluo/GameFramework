// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Skill/CoreAbilityCondition.h"
#include "CoreAbilityConditionGlobal.generated.h"

UCLASS(Category = "Ability", meta = (DisplayName = "前置连招检查", ToolTip = "前置连招检查", ShowDisplayNames), Abstract)
class GAMEFRAMEWORK_API UCoreAbilityCondition_CurrentComboSectionLimit : public UCoreAbilityCondition {
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SectionName;
};

UCLASS()
class GAMEFRAMEWORK_API UCoreAbilityConditionProgress_CurrentComboSectionLimit : public UCoreAbilityConditionProgress {
    GENERATED_BODY()

public:
    virtual bool IsComplete_Implementation(bool& IsValid) override;
};

UCLASS(Category = "Ability", meta = (DisplayName = "按下时间", ToolTip = "按下时间", ShowDisplayNames), Abstract)
class GAMEFRAMEWORK_API UCoreAbilityCondition_InputTime : public UCoreAbilityCondition {
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InputTimeLimit;
};

UCLASS()
class GAMEFRAMEWORK_API UCoreAbilityConditionProgress_InputTime : public UCoreAbilityConditionProgress {
    GENERATED_BODY()

public:
    virtual bool IsComplete_Implementation(bool& IsValid) override;
};