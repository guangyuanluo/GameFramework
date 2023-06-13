// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Skill/CoreAbilityCondition.h"
#include "GameplayTagContainer.h"
#include "CoreAbilityConditionGlobal.generated.h"

UCLASS(Category = "Ability", meta = (DisplayName = "前置连招检查", ToolTip = "前置连招检查", ShowDisplayNames), Abstract)
class GAMEFRAMEWORK_API UCoreAbilityCondition_CurrentComboSectionLimit : public UCoreAbilityCondition {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName SectionName;

    virtual void DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, bool& bValid, bool& bSatisfy) override;
};

UCLASS(Category = "Ability", meta = (DisplayName = "按下时间", ToolTip = "按下时间", ShowDisplayNames), Abstract)
class GAMEFRAMEWORK_API UCoreAbilityCondition_InputTime : public UCoreAbilityCondition {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float InputTimeLimit;

    virtual void DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, bool& bValid, bool& bSatisfy) override;
};