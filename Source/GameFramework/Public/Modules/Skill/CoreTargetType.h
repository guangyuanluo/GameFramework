// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Modules/Skill/CoreAbilityTypes.h"
#include "CoreTargetType.generated.h"

class ACoreCharacter;
class ACoreCharacterStateBase;
class AActor;
struct FGameplayEventData;

/**
 * Class that is used to determine targeting for abilities
 * It is meant to be blueprinted to run target logic
 * This does not subclass GameplayAbilityTargetActor because this class is never instanced into the world
 * This can be used as a basis for a game-specific targeting blueprint
 * If your targeting is more complicated you may need to instance into the world once or as a pooled actor
 */
UCLASS(Blueprintable, Abstract, meta = (ShowWorldContextPin))
class GAMEFRAMEWORK_API UCoreTargetType : public UObject
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UCoreTargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(ACoreCharacter* TargetingCharacter, ACoreCharacterStateBase* TargetingState, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable, meta = (DisplayName = "自身", ShowDisplayNames))
class GAMEFRAMEWORK_API UCoreTargetType_UseOwner : public UCoreTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UCoreTargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ACoreCharacter* TargetingCharacter, ACoreCharacterStateBase* TargetingState, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable, meta = (DisplayName = "事件对象", ShowDisplayNames))
class GAMEFRAMEWORK_API UCoreTargetType_UseEventData : public UCoreTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UCoreTargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ACoreCharacter* TargetingCharacter, ACoreCharacterStateBase* TargetingState, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};
