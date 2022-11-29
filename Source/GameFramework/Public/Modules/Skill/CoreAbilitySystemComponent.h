// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreAbilitySystemComponent.generated.h"

class UCoreAbility;

/**
 * Subclass of ability system component with game-specific data
 * Most games will need to make a game-specific subclass to provide utility functions
 */
UCLASS(ClassGroup = Skill, hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class GAMEFRAMEWORK_API UCoreAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    /**
    * 从模板初始化技能
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void InitSkillFromTemplate(int TemplateId);

    /** 返回指定条件拥有指定tag的技能 */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive = false, bool bOnlyAbilitiesThatSatisfyTagRequirements = true);

    /** 返回指定条件拥有指定类型的技能 */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void GetActiveAbilitiesWithClass(TSubclassOf<UGameplayAbility> AbilityClass, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive = false, bool bOnlyAbilitiesThatSatisfyTagRequirements = true);

    /**
    * 重置技能冷却
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void ResetAbilityCooldown(UGameplayAbility* Ability);

    /**
    * 技能连招
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void TryComboAbilityByClass(UCoreAbility* Ability);

    /**
    * 服务器端执行连招
    */
    UFUNCTION(Server, reliable, WithValidation)
    void ServerTryComboAbility(UCoreAbility* Ability);

private:
    void InternalComboAbility(UCoreAbility* Ability);
};