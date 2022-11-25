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
	* ¡¨’–≈‰÷√
	*/
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta = (Categories = "AbilityTagCategory"))
	TMap<FName, TSubclassOf<class UCoreAbilityComboExecutor>> ComboMap;
};