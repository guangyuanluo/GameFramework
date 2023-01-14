// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Structure/LogicObjectBase.h"
#include "Modules/Skill/CoreAbilityTypes.h"
#include "CoreAbilityComboExecutor.generated.h"

class UCoreAbility;

/**
* 连招执行器
*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UCoreAbilityComboExecutor : public ULogicObjectBase {
    GENERATED_BODY()

public:
    /**
    * 执行combo
    */
    UFUNCTION(BlueprintNativeEvent, Category = "Skill")
    void ExecuteCombo(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag);
};

/**
* 默认执行器
*/
UCLASS(meta = (DisplayName = "默认"))
class GAMEFRAMEWORK_API UCoreAbilityComboExecutor_Default : public UCoreAbilityComboExecutor {
    GENERATED_BODY()

public:
    /**
    * 执行combo
    */
    virtual void ExecuteCombo_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) override;
};