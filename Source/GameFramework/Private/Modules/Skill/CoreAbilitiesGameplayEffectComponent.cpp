// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Skill/CoreAbilitiesGameplayEffectComponent.h"

const TArray<FGameplayAbilitySpecConfig>& UCoreAbilitiesGameplayEffectComponent::GetGrantAbilityConfigs() const {
    return GrantAbilityConfigs;
}