// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreTargetType.h"
#include "CoreAbility.h"
#include "CoreCharacter.h"

void UCoreTargetType::GetTargets_Implementation(ACoreCharacter* TargetingCharacter, ACoreCharacterStateBase* TargetingState, FGameplayEventData EventData, const TArray<AActor*>& FilterActors, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}

void UCoreTargetType_UseOwner::GetTargets_Implementation(ACoreCharacter* TargetingCharacter, ACoreCharacterStateBase* TargetingState, FGameplayEventData EventData, const TArray<AActor*>& FilterActors, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	if (FilterActors.Contains(TargetingCharacter)) {
		return;
	}
	OutActors.Add(TargetingCharacter);
}

void UCoreTargetType_UseEventData::GetTargets_Implementation(ACoreCharacter* TargetingCharacter, ACoreCharacterStateBase* TargetingState, FGameplayEventData EventData, const TArray<AActor*>& FilterActors, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	bool AddTarget = false;
	if (EventData.TargetData.Num() > 0) {
		for (auto Index = 0; Index < EventData.TargetData.Num(); ++Index) {
			if (EventData.TargetData.IsValid(Index)) {
				auto TargetData = EventData.TargetData.Get(Index);
				if (const FHitResult* FoundHitResult = TargetData->GetHitResult()) {
					if (FilterActors.Contains(FoundHitResult->GetActor())) {
						continue;
					}
					AddTarget = true;
					OutHitResults.Add(*FoundHitResult);
				}
				else {
					for (auto Actor : TargetData->GetActors()) {
						if (FilterActors.Contains(Actor.Get())) {
							continue;
						}
						AddTarget = true;
						OutActors.Add(Actor.Get());
					}
				}
			}
		}
	}
	if (!AddTarget && EventData.Target) {
		if (FilterActors.Contains(EventData.Target)) {
			return;
		}
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}