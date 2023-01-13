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
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		if (FilterActors.Contains(FoundHitResult->GetActor())) {
			return;
		}
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		if (FilterActors.Contains(EventData.Target)) {
			return;
		}
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}