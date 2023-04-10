// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreAbilityInterface.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Modules/Skill/CoreAbilityTypes.h"
#include "CoreAbility.generated.h"

/** delegate define */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityInputPressed, class UCoreAbility*, Ability, int32, InputID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityInputReleased, class UCoreAbility*, Ability, int32, InputID);

/**
 * Subclass of ability blueprint type with game-specific data
 * This class uses GameplayEffectContainers to allow easier execution of gameplay effects based on a triggering tag
 * Most games will need to implement a subclass to support their game-specific code
 */
UCLASS(Abstract)
class GAMEFRAMEWORK_API UCoreAbility : public UGameplayAbility, public ICoreAbilityInterface
{
	GENERATED_UCLASS_BODY()

public:
    /**
    * 技能触发条件
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Default, meta = (DisplayName = "技能触发条件"))
    TArray<FCoreAbilityConditionGroupInfo> GroupConditionConfigs;

	/** gameplay tags 映射触发的 gameplay effect containers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default, meta = (DisplayName = "技能事件配置"))
	TMap<FGameplayTag, FCoreGameplayEffectContainer> EffectContainerMap;

	/**
    * 技能排序优先级
    */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default, meta = (DisplayName = "技能排序优先级"))
    int32 SortPriority;

    /**
     * 技能结束自动清空cd
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default", meta = (DisplayName = "技能结束自动清空cd"))
    bool bClearCooldownOnEnd = true;

    /**
    * 单次激活生效时长
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "生效限制", meta = (DisplayName = "限制时长", Tooltip = "单次激活生效时长"))
    float LimitActiveTime = 0.f;

    /**
    * 单次激活生效次数
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "生效限制", meta = (DisplayName = "限制次数", Tooltip = "单次激活生效次数"))
    int LimitActiveCounter = 0;

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

	/** Make gameplay effect container spec to be applied later, using the passed in container */
	UFUNCTION(BlueprintCallable, Category = Ability, meta=(AutoCreateRefTerm = "EventData"))
	virtual FCoreGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FCoreGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** Search for and make a gameplay effect container spec to be applied later, from the EffectContainerMap */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FCoreGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** Applies a gameplay effect container spec that was previously created */
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FCoreGameplayEffectContainerSpec& ContainerSpec);

	/** Applies a gameplay effect container, by creating and then applying the spec */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/**
	* 是否激活
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Ability, DisplayName = "IsActive", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool K2_IsActive() const;

    /**
    * 是否满足触发条件
    */
    UFUNCTION(BlueprintCallable, Category = Ability)
    bool K2_IsConditionSatisfy();

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

    virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const override;
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CallEndAbility() override;
	virtual void CallInputPressed(const FGameplayAbilitySpecHandle Handle);
	virtual void CallInputReleased(const FGameplayAbilitySpecHandle Handle);

private:
    FTimerHandle LimitActiveTimeHandle;
    TArray<FActiveGameplayEffectHandle> FollowGAPeriodEffectHandles;

    /**
    * 剩余计数
    */
    UPROPERTY()
    int RestCounter = 0;

    UFUNCTION()
    void LimitActiveTimeCallback();

    /**
    * 技能结束处理
    */
    UFUNCTION()
    void OnAbilityEnd(UGameplayAbility* Ability);

    /**
    * 技能条件节点信息
    */
    struct FAbilityConditionNodeInfo {
        bool IsRelationNode = false;
        CoreAbilityConditionRelationEnum Relation;
        int Index;
        TArray<FAbilityConditionNodeInfo> Children;
    };

    /** 按照运算符分割索引 */
    TArray<TArray<int>> SplitRelationIndexArray(CoreAbilityConditionRelationEnum Relation, const TArray<CoreAbilityConditionRelationEnum>& LoopRelations, const TArray<int>& RelationIndexs);
    /** 按运算符优先级递归构建逻辑树 */
    void RelationsGenerateRecursive(FAbilityConditionNodeInfo& NodeInfo, const TArray<CoreAbilityConditionRelationEnum>& RelationOrders, int NowOrderIndex, const TArray<CoreAbilityConditionRelationEnum>& LoopRelations, const TArray<int>& RelationIndexs);
    /** 构建运算符逻辑树 */
    FAbilityConditionNodeInfo RelationsGenerate(const TArray<CoreAbilityConditionRelationEnum>& LoopRelations);
    /** 递归一颗逻辑树的执行结果 */
    bool ExecuteConditionRelationTree(const FAbilityConditionNodeInfo& Node, const TFunction<bool(int)>& ConditionCheckFunc);
};