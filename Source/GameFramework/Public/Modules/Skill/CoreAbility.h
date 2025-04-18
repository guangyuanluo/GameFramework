// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "Modules/Skill/CoreAbilityTypes.h"
#include "Modules/TriggerAction/CoreConditionActionTrigger.h"
#include "CoreAbility.generated.h"

/** delegate define */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityInputPressed, class UCoreAbility*, Ability, int32, InputID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityInputReleased, class UCoreAbility*, Ability, int32, InputID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityFinish, class UCoreAbility*, Ability);

/**
 * Subclass of ability blueprint type with game-specific data
 * This class uses GameplayEffectContainers to allow easier execution of gameplay effects based on a triggering tag
 * Most games will need to implement a subclass to support their game-specific code
 */
UCLASS(Abstract)
class GAMEFRAMEWORK_API UCoreAbility : public UGameplayAbility
{
	GENERATED_UCLASS_BODY()

public:
    /**
    * 技能触发方式
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default, meta = (DisplayName = "技能触发方式"))
    CoreAbilityTriggerEnum TriggerWay = CoreAbilityTriggerEnum::E_InputKey;

    /**
    * 技能触发条件
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Default, meta = (DisplayName = "技能触发条件"))
    TArray<TObjectPtr<class UCoreCondition>> TriggerConditions;

    /**
    * 技能响应触发动作
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default, meta = (DisplayName = "技能响应触发动作"))
    TArray<FCoreConditionActionTriggerConfig> ConditionActionTriggerConfigs;

    /**
    * 技能额外终止条件
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Default, meta = (DisplayName = "技能额外终止条件"))
    TArray<TObjectPtr<class UCoreCondition>> ExternFinishConditions;

	/**
    * 技能排序优先级
    */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default, meta = (DisplayName = "技能排序优先级"))
    int32 SortPriority;

    /**
     * 技能结束自动清空cd
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default, meta = (DisplayName = "技能结束自动清空cd"))
    bool bClearCooldownOnEnd = false;

	/**
	* 按键按下通知
	*/
	UPROPERTY(BlueprintAssignable)
	FOnAbilityInputPressed OnAbilityInputPressed;

	/**
	* 按键释放通知
	*/
	UPROPERTY(BlueprintAssignable)
	FOnAbilityInputReleased OnAbilityInputReleased;

    /**
    * 技能结束
    */
    UPROPERTY(BlueprintAssignable)
    FOnAbilityFinish OnAbilityFinish;

	/** Make gameplay effect container spec to be applied later, using the passed in container */
	UFUNCTION(BlueprintCallable, Category = Ability, meta=(AutoCreateRefTerm = "EventData"))
	virtual FCoreGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FCoreGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** Applies a gameplay effect container spec that was previously created */
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FCoreGameplayEffectContainerSpec& ContainerSpec);

    UFUNCTION(BlueprintCallable, Category = Ability)
    void CallEndAbility();

    /**
    * 获得技能ID
    */
    UFUNCTION(BlueprintPure, Category = Ability)
    int32 GetSkillID() const;
    void SetSkillID(int32 InSkillID);

	/**
	* 是否激活
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Ability, DisplayName = "IsActive", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool K2_IsActive() const;

    /**
    * 是否满足触发条件
    */
    UFUNCTION(BlueprintCallable, Category = Ability)
    bool K2_IsConditionSatisfy() const;

    /**
    * 是否能重新触发
    */
    UFUNCTION(BlueprintPure, Category = Ability)
    bool K2_IsRetriggerInstancedAbility() const;

    /**
    * 技能激活
    */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Ability, DisplayName = "OnActivateNative", meta = (ScriptName = "OnActivateNative"))
    void OnActivateNative();

    /**
    * 技能结束
    */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Ability, DisplayName = "OnEndNative", meta = (ScriptName = "OnEndNative"))
    void OnEndNative();

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "InputPressed", meta = (ScriptName = "InputPressed"))
	void K2_InputPressed();

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "InputReleased", meta = (ScriptName = "InputReleased"))
	void K2_InputReleased();

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const override;
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CallInputPressed(const FGameplayAbilitySpecHandle Handle);
	virtual void CallInputReleased(const FGameplayAbilitySpecHandle Handle);

    void SetCurrentReceivedEventData(const FGameplayEventData& GameEventData);

protected:
    UPROPERTY(Transient)
    FGameplayEventData CurrentReceivedEventData;

private:
    UPROPERTY(Transient, Replicated)
    int32 SkillID;

    FTimerHandle LimitActiveTimeHandle;
    TArray<FActiveGameplayEffectHandle> FollowGAPeriodEffectHandles;

    /** 技能满足条件进度 */
    UPROPERTY(Transient)
    TArray<class UCoreConditionProgress*> RequireConditionProgresses;
    struct FConditionTriggerHandler PassiveConditionHandler;

    /** 技能触发信息 */
    UPROPERTY(Transient)
    TArray<FCoreConditionActionTriggerInfo> TriggerConditionProgressInfos;

    /** 额外技能终止条件进度 */
    UPROPERTY(Transient)
    TArray<class UCoreConditionProgress*> ExternFinishConditionProgresses;
    struct FConditionTriggerHandler ExternFinishConditionHandler;

    /**
    * 技能结束处理
    */
    UFUNCTION()
    void OnAbilityEnd(const FAbilityEndedData& AbilityEndedData);

    /**
    * 被动条件监听
    */
    UFUNCTION()
    void OnPassiveConditionTriggerCallback(FConditionTriggerHandler Handler);

    /**
    * 条件触发器监听
    */
    void StartConditionTriggerListen();
    void StopConditionTriggerListen();
    UFUNCTION()
    void OnConditionTriggerCallback(FConditionTriggerHandler Handler);
    /**
    * 额度终止条件监听
    */
    void StartExternFinishConditionListen();
    void StopExternFinishConditionListen();
    UFUNCTION()
    void OnExternFinishTriggerCallback(FConditionTriggerHandler Handler);

    /** 给进度赋值技能变量 */
    void SetProgressesWithAbility(const TArray<class UCoreConditionProgress*>& Progresses);
};