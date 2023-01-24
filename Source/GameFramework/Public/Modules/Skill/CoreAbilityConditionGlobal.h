// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Skill/CoreAbilityCondition.h"
#include "CoreAbilityConditionGlobal.generated.h"

UCLASS(meta = (DisplayName = "连招动画允许检查", ToolTip = "连招动画允许检查"))
class GAMEFRAMEWORK_API UCoreAbilityCondition_ComboEnable : public UCoreAbilityCondition {
    GENERATED_BODY()

public:
    virtual bool DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability) override;
};

UCLASS(meta = (DisplayName = "连招跳转技能合法性检查", ToolTip = "连招跳转技能合法性检查"))
class GAMEFRAMEWORK_API UCoreAbilityCondition_ComboJumpAbilityValid : public UCoreAbilityCondition {
    GENERATED_BODY()

public:
    virtual bool DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability) override;
};

UCLASS(meta = (DisplayName = "连招跳转片段合法性检查", ToolTip = "连招跳转片段合法性检查"))
class GAMEFRAMEWORK_API UCoreAbilityCondition_ComboJumpSectionValid : public UCoreAbilityCondition {
    GENERATED_BODY()

public:
    virtual bool DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability) override;
};

/**
* 触发方式Tag检查
*/
UCLASS(meta = (DisplayName = "触发方式Tag检查", ToolTip = "触发方式Tag检查"), Abstract)
class GAMEFRAMEWORK_API UCoreAbilityCondition_TriggerWayTag : public UCoreAbilityCondition {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag MatchTriggerTag;

    virtual bool DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability) override;
};