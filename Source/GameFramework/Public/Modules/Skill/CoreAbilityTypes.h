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

class UCoreTargetType;

/**
* 技能触发方式
*/
UENUM(BlueprintType)
enum class CoreAbilityTriggerEnum :uint8 {
    E_InputKey UMETA(DisplayName = "按键触发"),
    E_Passive UMETA(DisplayName = "被动触发"),
	E_SystemCall UMETA(DisplayName = "系统调用"),
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer, meta = (ShowDisplayNames))
	TSubclassOf<UCoreTargetType> TargetType;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FEffectInfo> TargetGameplayEffects;

    /**
    * 是否跟随技能生命周期
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer, meta = (DisplayName = "是否跟随技能生命周期"))
    bool FollowGAPeriod = false;

	/**
	 * 是否自动激活GE上的Granted GA
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer, meta = (DisplayName = "是否自动激活GE上的Granted GA"))
	bool AutoActiveGrantedAbility = false;
};

/** A "processed" version of CoreGameplayEffectContainer that can be passed around and eventually applied */
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FCoreGameplayEffectContainerSpec
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

    /** get all valid target actor */
    TArray<AActor*> GetAllTargetActors() const;

    /** clear targets */
    void ClearTargets();
};