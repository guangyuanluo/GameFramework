// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Skill/CoreAbilityCondition.h"
#include "CoreAbilityConditionGlobal.generated.h"

UCLASS(Category = "Ability", meta = (DisplayName = "前置连招检查", ToolTip = "前置连招检查", ShowDisplayNames))
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

UCLASS(Category = "Ability", meta = (DisplayName = "按下时间", ToolTip = "按下时间", ShowDisplayNames))
class GAMEFRAMEWORK_API UCoreAbilityCondition_InputTime : public UCoreAbilityCondition {
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InputID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InputTimeLimit;
};

UCLASS()
class GAMEFRAMEWORK_API UCoreAbilityConditionProgress_InputTime : public UCoreAbilityConditionProgress {
    GENERATED_BODY()

public:
    virtual bool IsComplete_Implementation(bool& IsValid) override;
};

UCLASS(Category = "Ability", meta = (DisplayName = "技能限制生效次数", ToolTip = "技能限制生效次数", ShowDisplayNames))
class GAMEFRAMEWORK_API UCoreAbilityCondition_AbilityLimitCounter : public UCoreAbilityCondition {
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LimitCounter;
};

UCLASS()
class GAMEFRAMEWORK_API UCoreAbilityConditionProgress_AbilityLimitCounter : public UCoreAbilityConditionProgress {
    GENERATED_BODY()

public:
    virtual void OnInitialize_Implementation() override;
    virtual void OnUninitialize_Implementation() override;
    virtual bool IsComplete_Implementation(bool& IsValid) override;

private:
    int CurrentCounter;

    UFUNCTION()
    void OnAbilityRestCounterUpdate(class UCoreAbilitySystemComponent* AbilitySystemComponent, class UCoreAbility* Ability, int NowCounter);
};