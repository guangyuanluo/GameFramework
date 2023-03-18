// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Structure/LogicObjectBase.h"
#include "CoreAbilityCondition.generated.h"

class UCoreAbility;

/**
* 技能条件
*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UCoreAbilityCondition : public ULogicObjectBase {
    GENERATED_BODY()

public:
    /**
    * 是否满足
    */
    UFUNCTION(BlueprintNativeEvent, Category = "Skill")
    void DoesSatisfy(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, bool& bValid, bool& bSatisfy);
};