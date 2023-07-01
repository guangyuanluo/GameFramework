// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "CoreAbilityCondition.generated.h"

/**
* 技能条件
*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UCoreAbilityCondition : public UCoreCondition {
    GENERATED_BODY()

public:
};

/**
* 技能条件进度 
*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UCoreAbilityConditionProgress : public UCoreConditionProgress {
    GENERATED_BODY()

public:
    /**
    * 获取条件关联的Ability
    */
    UFUNCTION(BlueprintCallable)
    class UCoreAbility* GetConditionAbility() const;
    
    UPROPERTY()
    TObjectPtr<class UCoreAbility> OwnerAbility;
};