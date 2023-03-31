// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreAbilityTypes.h"
#include "CoreAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

bool FCoreGameplayEffectContainerSpec::HasValidEffects() const
{
	return TargetGameplayEffectSpecs.Num() > 0;
}

bool FCoreGameplayEffectContainerSpec::HasValidTargets() const
{
	return TargetData.Num() > 0;
}

void FCoreGameplayEffectContainerSpec::AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	for (const FHitResult& HitResult : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		TargetData.Add(NewData);
	}

	if (TargetActors.Num() > 0)
	{
		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Append(TargetActors);
		TargetData.Add(NewData);
	}
}

TArray<AActor*> FCoreGameplayEffectContainerSpec::GetAllTargetActors() const {
    TSet<AActor*> ActorSet;
    TArray<AActor*> Result;

    for (const auto& Data : TargetData.Data) {
        auto DataActors = Data->GetActors();
        for (const auto& DataActor : DataActors) {
            if (DataActor.IsValid()) {
                if (!ActorSet.Contains(DataActor.Get())) {
                    ActorSet.Add(DataActor.Get());
                    Result.Add(DataActor.Get());
                }
            }
        }
    }

    return Result;
}

void FCoreGameplayEffectContainerSpec::ClearTargets() {
    TargetData.Clear();
}