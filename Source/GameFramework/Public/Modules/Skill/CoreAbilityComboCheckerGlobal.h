// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Skill/CoreAbilityComboChecker.h"
#include "CoreAbilityComboCheckerGlobal.generated.h"

/**
* 连招检查器
*/
UCLASS(meta = (DisplayName = "触发方式Tag检查"))
class GAMEFRAMEWORK_API UCoreAbilityComboChecker_TriggerWayTag : public UCoreAbilityComboChecker {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag MatchTriggerTag;

    virtual bool CanComboExecute_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) override;
};

UCLASS(meta = (DisplayName = "动画允许连招检查"))
class GAMEFRAMEWORK_API UCoreAbilityComboChecker_ComboEnable : public UCoreAbilityComboChecker {
    GENERATED_BODY()

public:
    virtual bool CanComboExecute_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) override;
};

UCLASS(meta = (DisplayName = "跳转技能合法性检查"))
class GAMEFRAMEWORK_API UCoreAbilityComboChecker_JumpAbilityValid : public UCoreAbilityComboChecker {
    GENERATED_BODY()

public:
    virtual bool CanComboExecute_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) override;
};

UCLASS(meta = (DisplayName = "跳转片段合法性检查"))
class GAMEFRAMEWORK_API UCoreAbilityComboChecker_JumpSectionValid : public UCoreAbilityComboChecker {
    GENERATED_BODY()

public:
    virtual bool CanComboExecute_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) override;
};