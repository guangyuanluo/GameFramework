// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "CoreAbility.h"
#include "AbilitySystemComponent.h"
#include "CoreConditionGroup.h"
#include "CurrentComboSectionLimitCondition.h"
#include "SkillSetting.h"

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

	UCoreAbility* TriggerAbility = nullptr;
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs) {
		if (SpecHandle.IsValid()) {
			auto Ability = SpecHandle.Data->GetEffectContext().GetAbilityInstance_NotReplicated();
			if (Ability->IsA(UCoreAbility::StaticClass())) {
				TriggerAbility = const_cast<UCoreAbility*>(Cast<UCoreAbility>(Ability));
			}
			break;
		}
	}

	// Iterate list of gameplay effects
	if (TriggerAbility) {
		AllEffects = TriggerAbility->ApplyEffectContainerSpec(ContainerSpec);
	}
	else {
		for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs) {
			if (SpecHandle.IsValid()) {
				// If effect is valid, iterate list of targets and apply to all
				for (TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data) {
					AllEffects.Append(Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get()));
				}
			}
		}
	}
	return AllEffects;
}

bool USkillBlueprintLibrary::IsAbilityMatchingAllTags(UGameplayAbility* Ability, FGameplayTagContainer AbilityTags) {
	return Ability->GetAssetTags().HasAll(AbilityTags);
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
	if (!Ability) {
		return false;
	}
	const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
	if (Ability->GetAssetTags().HasTag(SkillSetting->ComboSkillTag)) {
		return true;
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

const UGameplayAbility* USkillBlueprintLibrary::EffectContextGetAbilityCDO(const FGameplayEffectContextHandle& EffectContext) {
	return EffectContext.GetAbility();
}

const UGameplayAbility* USkillBlueprintLibrary::EffectContextGetAbilityInstance_NotReplicated(const FGameplayEffectContextHandle& EffectContext) {
	return EffectContext.GetAbilityInstance_NotReplicated();
}