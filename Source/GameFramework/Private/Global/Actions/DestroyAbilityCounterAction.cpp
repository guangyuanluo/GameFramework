// Fill out your copyright notice in the Description page of Project Settings.

#include "DestroyAbilityCounterAction.h"
#include "CoreAbility.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "Modules/AbilityCounter/AbilityCounterSubsystem.h"

void UDestroyAbilityCounterAction::OnExecute_Implementation(const FGameplayEventData& EventData) const {
    auto Ability = GetExecuteAbility(EventData);
    if (!Ability) {
        return;
    }
    auto OwnerActor = Ability->GetOwningActorFromActorInfo();
    auto AbilityCounterSubsystem = Cast<UAbilityCounterSubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(OwnerActor, UAbilityCounterSubsystem::StaticClass()));
    if (IsGlobal) {
        AbilityCounterSubsystem->DestroyCounter(CounterName);
    }
    else {
        AbilityCounterSubsystem->DestroyCounter(FString::Format(TEXT("{0}_{1}"), { *CounterName, *OwnerActor->GetName() }));
    }
}