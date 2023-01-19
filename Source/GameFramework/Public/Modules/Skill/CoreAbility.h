// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayAbility.h"
#include "CoreAbilityInterface.h"
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
	* 连招配置
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default, meta = (DisplayName = "连招配置"))
	TMap<FName, FComboSectionConfigs> ComboMap;

	/** gameplay tags 映射触发的 gameplay effect containers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default, meta = (DisplayName = "技能事件配置"))
	TMap<FGameplayTag, FCoreGameplayEffectContainer> EffectContainerMap;

	/**
	* 连招执行器
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "连招执行器"))
	TSubclassOf<class UCoreAbilityComboExecutor> ComboExecutor;

	/**
    * 技能排序优先级
    */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default, meta = (DisplayName = "技能排序优先级"))
    int32 SortPriority;

	/**
	* 需要过滤处理的actor，避免多次伤害计算
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> FilterActors;

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
	* 清空过滤的actor
	*/
	UFUNCTION(BlueprintCallable, Category = Ability)
	void ClearFilterActors();

	/**
	* 是否激活
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Ability, DisplayName = "IsActive", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool K2_IsActive() const;

	/**
	* 触发连招
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Ability, DisplayName = "ComboAbility", meta = (ScriptName = "ComboAbility"))
	void NotifyComboAbility(class UCoreAbilitySystemComponent* AbilityComponent, FName const ComboSection);

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "InputPressed", meta = (ScriptName = "InputPressed"))
	void K2_InputPressed();

	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "InputReleased", meta = (ScriptName = "InputReleased"))
	void K2_InputReleased();

	virtual void CallEndAbility() override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
};