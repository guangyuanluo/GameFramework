// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "Modules/Skill/SkillInfo.h"
#include "Modules/Skill/EffectInfo.h"
#include "CoreAbilitySystemComponent.generated.h"

class UCoreAbility;

DECLARE_MULTICAST_DELEGATE_OneParam(FSkillTemplatePostInit, class UCoreAbilitySystemComponent*);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEffectPreAddDynDelegate, class UCoreAbilitySystemComponent*, SkillComponent, const FGameplayEffectSpecHandle&, Spec, TSubclassOf<class UGameplayEffect>, EffectClass);
DECLARE_DELEGATE_ThreeParams(FEffectPreAddDelegate, class UCoreAbilitySystemComponent*, const FGameplayEffectSpecHandle&, TSubclassOf<class UGameplayEffect>);

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

    /**
    * 增加技能模组
    */
    UFUNCTION(BlueprintCallable, Category = "Character", meta = (DisplayName = "AddSkillGroup", AutoCreateRefTerm = "InEffectPreAddCallback"))
    virtual void K2_AddSkillGroup(int SkillGroupID, const FEffectPreAddDynDelegate& InEffectPreAddCallback);

    virtual void AddSkillGroup(int SkillGroupID, const FEffectPreAddDelegate& InEffectPreAddCallback);

    /**
    * 移除技能模组
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void RemoveSkillGroup(int SkillGroupID);

    /**
    * 增加技能
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void AddSkill(const FSkillInfo& SkillInfo);

    /**
    * 移除技能
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void RemoveSkill(const FSkillInfo& SkillInfo);

    /**
    * 增加效果
    */
    UFUNCTION(BlueprintCallable, Category = "Character", meta = (DisplayName = "AddEffect", AutoCreateRefTerm = "InEffectPreAddCallback"))
    virtual void K2_AddEffect(const FEffectInfo& EffectInfo, const FEffectPreAddDynDelegate& InEffectPreAddCallback);

    virtual void AddEffect(const FEffectInfo& EffectInfo, const FEffectPreAddDelegate& InEffectPreAddCallback);

    /**
    * 移除效果
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void RemoveEffect(const FEffectInfo& EffectInfo);

    /** 返回指定条件拥有指定tag的技能 */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive = false, bool bOnlyAbilitiesThatSatisfyTagRequirements = true);

    /** 返回指定条件拥有指定类型的技能 */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void GetActiveAbilitiesWithClass(TSubclassOf<UGameplayAbility> AbilityClass, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive = false, bool bOnlyAbilitiesThatSatisfyTagRequirements = true);

    /** 返回指定条件拥有指定InputID的技能 */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void GetActiveAbilitiesWithInputID(int32 InputID, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive = false, bool bOnlyAbilitiesThatSatisfyTagRequirements = true);

    /**
    * 重置技能冷却
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void ResetAbilityCooldown(UGameplayAbility* Ability);

    /**
    * 技能连招
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void TryComboAbilityByClass(UCoreAbility* Ability, FGameplayTag TriggerWayTag);

    /**
    * 服务器端执行连招
    */
    UFUNCTION(Server, reliable, WithValidation)
    void ServerTryComboAbility(FGameplayAbilitySpecHandle AbilityToCombo, FGameplayTag TriggerWayTag);

    /**
    * 技能模板初始化委托
    */
    FSkillTemplatePostInit OnSkillTemplatePostInit;

private:
    void InternalComboAbility(UCoreAbility* Ability, FGameplayTag TriggerWayTag);

    void AddSkillPrivate(class UDataTable* SkillDataTable, const FSkillInfo& SkillInfo);
    void RemoveSkillPrivate(class UDataTable* SkillDataTable, const FSkillInfo& SkillInfo);
    void AddEffectPrivate(class UDataTable* EffectDataTable, const FEffectInfo& EffectInfo, const FEffectPreAddDelegate& InEffectPreAddCallback);
    void RemoveEffectPrivate(class UDataTable* EffectDataTable, const FEffectInfo& EffectInfo);
};