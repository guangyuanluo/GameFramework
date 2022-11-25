// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreAbilityComboExecutor.generated.h"

class UCoreAbility;
/**
* ����������
*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UCoreAbilityComboExecutor : public UObject {
    GENERATED_BODY()

public:
    /**
    * ����ܷ�ִ��combo
    */
    UFUNCTION(BlueprintNativeEvent, Category = "Skill")
    bool CheckComboEnable(UCoreAbility* Ability);

    /**
    * ִ��combo
    */
    UFUNCTION(BlueprintNativeEvent, Category = "Skill")
    void ExecuteCombo(UCoreAbility* Ability);
};