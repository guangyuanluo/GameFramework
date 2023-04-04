// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "CoreAbility.h"
#include "Modules/Skill/CoreAbilityConditionGlobal.h"

bool USkillBlueprintLibrary::DoesEffectContainerSpecHaveEffects(const FCoreGameplayEffectContainerSpec& ContainerSpec) {
    return ContainerSpec.HasValidEffects();
}

FCoreGameplayEffectContainerSpec USkillBlueprintLibrary::AddTargetsToEffectContainerSpec(const FCoreGameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors) {
	FCoreGameplayEffectContainerSpec NewSpec = ContainerSpec;
	NewSpec.AddTargets(HitResults, TargetActors);
	return NewSpec;
}

TArray<FActiveGameplayEffectHandle> USkillBlueprintLibrary::ApplyExternalEffectContainerSpec(const FCoreGameplayEffectContainerSpec& ContainerSpec) {
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of gameplay effects
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs) {
		if (SpecHandle.IsValid()) {
			// If effect is valid, iterate list of targets and apply to all
			for (TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data) {
				AllEffects.Append(Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get()));
			}
		}
	}
	return AllEffects;
}

bool USkillBlueprintLibrary::IsAbilityMatchingAllTags(UGameplayAbility* Ability, FGameplayTagContainer AbilityTags) {
	return Ability->AbilityTags.HasAll(AbilityTags);
}

float USkillBlueprintLibrary::GetSetByCallerMagnitudeWithSpecHandle(FGameplayEffectSpecHandle SpecHandle, FGameplayTag DataTag, bool WarnIfNotFound, float DefaultIfNotFound) {
    FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
    if (Spec) {
        return Spec->GetSetByCallerMagnitude(DataTag, WarnIfNotFound, DefaultIfNotFound);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("SpecHandle Not Valid"));
    }

    return 0.f;
}

float USkillBlueprintLibrary::GetSetByCallerMagnitudeWithSpec(FGameplayEffectSpec Spec, FGameplayTag DataTag, bool WarnIfNotFound, float DefaultIfNotFound) {
    return Spec.GetSetByCallerMagnitude(DataTag, WarnIfNotFound, DefaultIfNotFound);
}

bool USkillBlueprintLibrary::IsComboAbility(const UCoreAbility* Ability) {
	for (const auto& ConditionConfig : Ability->ConditionConfigs) {
		if (ConditionConfig.Condition->IsChildOf(UCoreAbilityCondition_CurrentComboSectionLimit::StaticClass())) {
			return true;
		}
	}
	return false;
}

ACoreCharacter* USkillBlueprintLibrary::GetCharacterFromGameEffectSpec(const FGameplayEffectSpec& Spec) {
    auto EffectCauser = Spec.GetContext().GetEffectCauser();
    if (EffectCauser) {
        auto CoreCharacter = Cast<ACoreCharacter>(EffectCauser);
        return CoreCharacter;
    }
    return nullptr;
}

UGameplayAbility* USkillBlueprintLibrary::GetActiveAbilityWithCurrentMontage(UAbilitySystemComponent* AbilitySystemComponent, UAnimMontage* Montage) {
	const auto& ActivatableAbilities = AbilitySystemComponent->GetActivatableAbilities();
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities) {
		TArray<UGameplayAbility*> AbilityInstances = Spec.GetAbilityInstances();
		for (UGameplayAbility* AbilityInstance : AbilityInstances) {
			if (AbilityInstance->IsActive()
				&& AbilityInstance->GetCurrentMontage() == Montage) {
				return AbilityInstance;
			}
		}
	}
	return nullptr;
}