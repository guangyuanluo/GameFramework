// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Modules/Skill/CoreAbilityTypes.h"
#include "GameplayEffect.h"
#include "SkillBlueprintLibrary.generated.h"


/**
 * 技能公共函数
 */
UCLASS()
class GAMEFRAMEWORK_API USkillBlueprintLibrary : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	/** Checks if spec has any effects */
	UFUNCTION(BlueprintPure, Category = Ability)
	static bool DoesEffectContainerSpecHaveEffects(const FCoreGameplayEffectContainerSpec& ContainerSpec);

	/** Adds targets to a copy of the passed in effect container spec and returns it */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "HitResults,TargetActors"))
	static FCoreGameplayEffectContainerSpec AddTargetsToEffectContainerSpec(const FCoreGameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);

	/** Applies container spec that was made from an ability */
	UFUNCTION(BlueprintCallable, Category = Ability)
	static TArray<FActiveGameplayEffectHandle> ApplyExternalEffectContainerSpec(const FCoreGameplayEffectContainerSpec& ContainerSpec);

	/** check whether the ability match all tags */
	UFUNCTION(BlueprintCallable, Category = Ability)
	static bool IsAbilityMatchingAllTags(UGameplayAbility* Ability, FGameplayTagContainer AbilityTags);

    UFUNCTION(BlueprintCallable, Category = Ability)
    static float GetSetByCallerMagnitudeWithSpecHandle(struct FGameplayEffectSpecHandle SpecHandle, FGameplayTag DataTag, bool WarnIfNotFound = true, float DefaultIfNotFound = 0.f);

    UFUNCTION(BlueprintCallable, Category = Ability)
    static float GetSetByCallerMagnitudeWithSpec(struct FGameplayEffectSpec Spec, FGameplayTag DataTag, bool WarnIfNotFound = true, float DefaultIfNotFound = 0.f);

	UFUNCTION(BlueprintCallable, Category = Ability)
	static bool IsComboAbility(const UCoreAbility* Ability);

	/**
	* 从技能效果中获得单位
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static class ACoreCharacter* GetCharacterFromGameEffectSpec(const FGameplayEffectSpec& Spec);

	/** 返回已经激活的拥有指定蒙太奇的技能 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	static class UGameplayAbility* GetActiveAbilityWithCurrentMontage(UAbilitySystemComponent* AbilitySystemComponent, class UAnimMontage* Montage);

	/** 从effectcontext里面获取AbilityCDO */
	UFUNCTION(BlueprintPure, Category = "Ability|EffectContext", Meta = (DisplayName = "GetAbilityCDO"))
	static const UGameplayAbility* EffectContextGetAbilityCDO(const FGameplayEffectContextHandle& EffectContext);

	/** 从effectcontext里面获取Ability Instance */
	UFUNCTION(BlueprintPure, Category = "Ability|EffectContext", Meta = (DisplayName = "GetAbilityInstance_NotReplicated"))
	static const UGameplayAbility* EffectContextGetAbilityInstance_NotReplicated(const FGameplayEffectContextHandle& EffectContext);
};