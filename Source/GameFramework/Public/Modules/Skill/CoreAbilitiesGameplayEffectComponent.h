// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectComponents/AbilitiesGameplayEffectComponent.h"
#include "CoreAbilitiesGameplayEffectComponent.generated.h"

/**
 * Grants additional Gameplay Abilities to the Target of a Gameplay Effect while active
 */
UCLASS()
class GAMEFRAMEWORK_API UCoreAbilitiesGameplayEffectComponent : public UAbilitiesGameplayEffectComponent
{
	GENERATED_BODY()

public:
	const TArray<FGameplayAbilitySpecConfig>& GetGrantAbilityConfigs() const;
};
