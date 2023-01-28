// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// ----------------------------------------------------------------------------------------------------------------
// This header is for Ability-specific structures and enums that are shared across a project
// Every game will probably need a file like this to handle their extensions to the system
// This file is a good place for subclasses of FGameplayEffectContext and FGameplayAbilityTargetData
// ----------------------------------------------------------------------------------------------------------------

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Modules/Skill/EffectInfo.h"
#include "CoreAbilityTypes.generated.h"

class UCoreAbilitySystemComponent;
class UGameplayEffect;
class UCoreTargetType;
class UCoreAbility;

UENUM(BlueprintType)
enum class CoreAbilityCounterEnum :uint8 {
    E_None UMETA(DisplayName = "不消耗"),
    E_Fixed UMETA(DisplayName = "固定次数"),
    E_Trigger UMETA(DisplayName = "触发次数"),
};

/**
 * Struct defining a list of gameplay effects, a tag, and targeting info
 * These containers are defined statically in blueprints or assets and then turn into Specs at runtime
 */
USTRUCT(BlueprintType)
struct FCoreGameplayEffectContainer
{
	GENERATED_BODY()

public:
	FCoreGameplayEffectContainer() {}

	/** Sets the way that targeting happens */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TSubclassOf<UCoreTargetType> TargetType;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FEffectInfo> TargetGameplayEffects;

#if WITH_EDITORONLY_DATA
    UPROPERTY(EditAnywhere, AdvancedDisplay, Category = GameplayEffectContainer, meta = (DisplayName = "是否显示计数相关属性"))
    bool ShowCounterEdit = false;
#endif

    /**
    * 消耗计数器类型
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer, meta = (DisplayName = "计数消耗类型"))
    CoreAbilityCounterEnum CounterEnum = CoreAbilityCounterEnum::E_None;

    /**
    * 如果是固定消耗类型，这里填固定消耗的数量
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer, meta = (DisplayName = "固定消耗次数"))
    int FixedCounter;

    /**
    * 是否跟随技能生命周期
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer, meta = (DisplayName = "是否跟随技能生命周期"))
    bool FollowGAPeriod = false;
};

/** A "processed" version of CoreGameplayEffectContainer that can be passed around and eventually applied */
USTRUCT(BlueprintType)
struct FCoreGameplayEffectContainerSpec
{
	GENERATED_BODY()

public:
	FCoreGameplayEffectContainerSpec() {}

	/** Computed target data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

    FCoreGameplayEffectContainer* ContainerPtr;

	/** Returns true if this has any valid effect specs */
	bool HasValidEffects() const;

	/** Returns true if this has any valid targets */
	bool HasValidTargets() const;

	/** Adds new targets to target data */
	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};

/**
* 连招片段配置
*/
USTRUCT(BlueprintType, meta = (DisplayName = "配置项"))
struct FComboSectionConfig {
	GENERATED_BODY()

	/**
	* 跳转蒙太奇片段
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "跳转蒙太奇片段"))
	FName JumpSection;

	/**
	* 跳转技能
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "跳转其他技能"))
	TSubclassOf<UCoreAbility> JumpAbility;

	/**
	* 连招检查器
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "连招条件"))
	TArray<TSubclassOf<class UCoreAbilityCondition>> Conditions;
};

/**
* 连招片段配置
*/
USTRUCT(BlueprintType)
struct FComboSectionConfigs {
	GENERATED_BODY()

	/**
	* 所有配置
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FComboSectionConfig> Configs;
};