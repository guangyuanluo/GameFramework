// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Structure/LogicObjectBase.h"
#include "Modules/Skill/CoreAbilityTypes.h"
#include "CoreAbilityComboChecker.generated.h"

class UCoreAbility;

/**
* 连招检查器
*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UCoreAbilityComboChecker : public ULogicObjectBase {
    GENERATED_BODY()

public:
    /**
    * 检查能否执行combo
    */
    UFUNCTION(BlueprintNativeEvent, Category = "Skill")
    bool CanComboExecute(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag);
};