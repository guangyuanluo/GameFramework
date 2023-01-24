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
};