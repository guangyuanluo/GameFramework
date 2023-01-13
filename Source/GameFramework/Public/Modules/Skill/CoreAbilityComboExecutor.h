// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Structure/LogicObjectBase.h"
#include "CoreAbilityComboExecutor.generated.h"

class UCoreAbility;
/**
* 连招数据项
*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UCoreAbilityComboExecutor : public ULogicObjectBase {
    GENERATED_BODY()

public:
    /**
    * 检查能否执行combo
    */
    UFUNCTION(BlueprintNativeEvent, Category = "Skill")
    bool CanComboExecute(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, FGameplayTag TriggerWayTag);

    /**
    * 执行combo
    */
    UFUNCTION(BlueprintNativeEvent, Category = "Skill")
    void ExecuteCombo(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, FGameplayTag TriggerWayTag);
};