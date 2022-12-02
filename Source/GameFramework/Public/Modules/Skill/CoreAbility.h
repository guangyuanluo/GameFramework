// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayAbility.h"
#include "CoreAbility.generated.h"

/**
 * Subclass of ability blueprint type with game-specific data
 * This class uses GameplayEffectContainers to allow easier execution of gameplay effects based on a triggering tag
 * Most games will need to implement a subclass to support their game-specific code
 */
UCLASS()
class GAMEFRAMEWORK_API UCoreAbility : public UGameplayAbility
{
	GENERATED_UCLASS_BODY()

public:
	/**
	* 连招配置
	*/
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta = (Categories = "AbilityTagCategory"))
	TMap<FName, TSubclassOf<class UCoreAbilityComboExecutor>> ComboMap;

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
};