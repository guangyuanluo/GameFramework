// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "Modules/Skill/SkillInfo.h"
#include "Modules/Skill/EffectInfo.h"
#include "CoreAbilitySystemComponent.generated.h"

class UCoreAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillTemplatePostInit, class UCoreAbilitySystemComponent*, SkillComponent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEffectPreAddDynMutiDelegate, class UCoreAbilitySystemComponent*, SkillComponent, const FGameplayEffectSpecHandle&, Spec, TSubclassOf<class UGameplayEffect>, EffectClass);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEffectPreAddDynDelegate, class UCoreAbilitySystemComponent*, SkillComponent, const FGameplayEffectSpecHandle&, Spec, TSubclassOf<class UGameplayEffect>, EffectClass);
DECLARE_DELEGATE_ThreeParams(FEffectPreAddDelegate, class UCoreAbilitySystemComponent*, const FGameplayEffectSpecHandle&, TSubclassOf<class UGameplayEffect>);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAbilityRestCounterUpdateDelegate, class UCoreAbilitySystemComponent*, UCoreAbility*, int);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillAddDelegate, class UCoreAbilitySystemComponent*, SkillComponent, UGameplayAbility*, AbilityAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillRemoveDelegate, class UCoreAbilitySystemComponent*, SkillComponent, UGameplayAbility*, AbilityRemoved);

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
    * effect添加前处理
    */
    UPROPERTY(BlueprintAssignable)
    FEffectPreAddDynMutiDelegate EffectPreAddDynMutiDelegate;

    /**
    * 技能增加处理
    */
    UPROPERTY(BlueprintAssignable)
    FOnSkillAddDelegate OnSkillAddDelegate;

    /**
    * 技能移除处理
    */
    UPROPERTY(BlueprintAssignable)
    FOnSkillRemoveDelegate OnSkillRemoveDelegate;

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

    /** 判断一个技能是否符合条件 */
    UFUNCTION(BlueprintCallable, Category = "Character")
    bool DoesAbilityFilterCondition(const FGameplayTagContainer& TargetTagContainer, UCoreAbility* ActiveAbility, bool ForceFilterActive, bool bOnlyAbilitiesThatSatisfy);

    /** 返回指定条件拥有指定tag的技能 */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive = false, bool bOnlyAbilitiesThatSatisfy = true, bool SortByOrder = true);

    /** 返回指定条件拥有指定类型的技能 */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void GetActiveAbilitiesWithClass(TSubclassOf<UGameplayAbility> AbilityClass, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive = false, bool bOnlyAbilitiesThatSatisfy = true, bool SortByOrder = true);

    /** 返回指定条件拥有指定InputID的技能 */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void GetActiveAbilitiesWithInputID(int32 InputID, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive = false, bool bOnlyAbilitiesThatSatisfy = true, bool SortByOrder = true);

    /** 返回指定条件拥有指定InputID的优先级最高技能class */
    UFUNCTION(BlueprintCallable, Category = "Character")
    UCoreAbility* GetActiveAbilityWithInputID(int32 InputID, bool ForceFilterActive = false, bool bOnlyAbilitiesThatSatisfy = true);

    /** 返回用来运行的指定条件拥有指定InputID的优先级最高技能class */
    UFUNCTION(BlueprintCallable, Category = "Character")
    TSubclassOf<UCoreAbility> GetActiveAbilityToRunWithInputID(int32 InputID);

    /**
    * 重置技能冷却
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void ResetAbilityCooldown(UGameplayAbility* Ability);

    /**
    * 清空技能冷却
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void ClearAbilityCooldown(UGameplayAbility* Ability);

    /**
    * 按下对应input
    */
    UFUNCTION(BlueprintCallable, Category = "Character", meta = (DisplayName = "AbilityLocalInputPressed"))
    void K2_AbilityLocalInputPressed(int32 InputID);

    /**
    * 释放对应input
    */
    UFUNCTION(BlueprintCallable, Category = "Character", meta = (DisplayName = "AbilityLocalInputReleased"))
    void K2_AbilityLocalInputReleased(int32 InputID);

    /**
     * 获取按键按下时间(秒)
     */
    UFUNCTION(BlueprintCallable, Category = "Character")
    float GetInputPressTime(int32 InputID) const;

    /**
    * 设置限制技能生效次数
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void SetAbilityLimitCounter(UCoreAbility* Ability, int LimitCounter);

    /**
    * 移除限制技能生效次数
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void RemoveAbilityLimitCounter(UCoreAbility* Ability);

    /**
    * 变更技能生效次数
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    void ChangeAbilityRestCounter(UCoreAbility* Ability, int Num);

    /**
    * 给客户端的actor发送
    */
    UFUNCTION(BlueprintCallable, Category = "Character", Client, Reliable)
    void Client_SendGameplayEventToActor(FGameplayTag EventTag, FGameplayEventData Payload);

    /**
    * 给客户端跳转蒙太奇
    */
    UFUNCTION(BlueprintCallable, Category = "Character", NetMulticast, Reliable)
    void Multicast_CurrentMontageJumpToSection(class UAnimMontage* InMontage, FName SectionName);

    UFUNCTION(BlueprintCallable, Category = "Character")
    UGameplayAbility* K2_GetAnimatingAbility();

    UFUNCTION(BlueprintCallable, Category = "Character")
    UAnimMontage* K2_GetCurrentMontage() const;

    /**
    * 技能模板初始化委托
    */
    UPROPERTY(BlueprintAssignable)
    FSkillTemplatePostInit OnSkillTemplatePostInit;
    /**
    * 技能生效次数变革委托
    */
    FOnAbilityRestCounterUpdateDelegate OnAbilityRestCounterUpdateDelegate;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
    virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
    virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;

private:
    TMap<int32, FDateTime> InputTimeMap;

    UPROPERTY()
    TMap<UCoreAbility*, int> AbilityRestCounterMap;

    UPROPERTY()
    TArray<UCoreAbility*> SortCoreAbilities;

    void AddSkillPrivate(class UDataTable* SkillDataTable, const FSkillInfo& SkillInfo);
    void RemoveSkillPrivate(class UDataTable* SkillDataTable, const FSkillInfo& SkillInfo);
    void AddEffectPrivate(class UDataTable* EffectDataTable, const FEffectInfo& EffectInfo, const FEffectPreAddDelegate& InEffectPreAddCallback);
    void RemoveEffectPrivate(class UDataTable* EffectDataTable, const FEffectInfo& EffectInfo);

    UFUNCTION(Server, reliable, WithValidation)
    void ServerInputPressed(int32 InputID);

    UFUNCTION(Server, reliable, WithValidation)
    void ServerInputReleased(int32 InputID);

    void InputPressedLocal(int32 InputID);
    void InputReleasedLocal(int32 InputID);

    FGameplayTagContainer GetTargetTagContainer();

    void OnActiveEffectAdded(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
    void OnActiveEffectRemove(const struct FActiveGameplayEffect& FGameplayEffectRemovalInfo);
};