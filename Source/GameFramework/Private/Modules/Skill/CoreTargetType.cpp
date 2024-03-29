// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreTargetType.h"
#include "CoreAbility.h"
#include "CoreCharacter.h"

void UCoreTargetType::GetTargets(class UCoreAbility* Ability, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	K2_GetTargets(Ability, EventData, OutHitResults, OutActors);
}

void UCoreTargetType_UseOwner::GetTargets(class UCoreAbility* Ability, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(Ability->GetAvatarActorFromActorInfo());
}

void UCoreTargetType_UseEventData::GetTargets(class UCoreAbility* Ability, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	bool AddTarget = false;
	if (EventData.TargetData.Num() > 0) {
		for (auto Index = 0; Index < EventData.TargetData.Num(); ++Index) {
			if (EventData.TargetData.IsValid(Index)) {
				auto TargetData = EventData.TargetData.Get(Index);
				if (const FHitResult* FoundHitResult = TargetData->GetHitResult()) {
					AddTarget = true;
					OutHitResults.Add(*FoundHitResult);
				}
				else {
					for (auto Actor : TargetData->GetActors()) {
						AddTarget = true;
						OutActors.Add(Actor.Get());
					}
				}
			}
		}
	}
}