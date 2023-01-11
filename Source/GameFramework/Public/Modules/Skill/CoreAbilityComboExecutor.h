// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/Structure/LogicObjectBase.h"
#include "CoreAbilityComboExecutor.generated.h"

class UCoreAbility;
/**
* ����������
*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UCoreAbilityComboExecutor : public ULogicObjectBase {
    GENERATED_BODY()

public:
    /**
    * ����ܷ�ִ��combo
    */
    UFUNCTION(BlueprintNativeEvent, Category = "Skill")
    bool CheckComboEnable(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability);

    /**
    * ִ��combo
    */
    UFUNCTION(BlueprintNativeEvent, Category = "Skill")
    void ExecuteCombo(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability);
};