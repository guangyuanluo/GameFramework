// Fill out your copyright notice in the Description page of Project Settings.

#include "ChangeAbilityCounterAction.h"
#include "CoreAbility.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "Modules/AbilityCounter/AbilityCounterSubsystem.h"

void UChangeAbilityCounterAction::OnExecute(const FGameplayEventData& EventData) const {
    auto Ability = GetExecuteAbility(EventData);
    if (!Ability) {
        return;
    }
    auto OwnerActor = Ability->GetOwningActorFromActorInfo();
    auto AbilityCounterSubsystem = Cast<UAbilityCounterSubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(OwnerActor, UAbilityCounterSubsystem::StaticClass()));
    if (IsGlobal) {
        AbilityCounterSubsystem->ChangeCounterNum(CounterName, DiffNum);
    }
    else {
        AbilityCounterSubsystem->ChangeCounterNum(FString::Format(TEXT("{0}_{1}"), { *CounterName, *OwnerActor->GetName() }), DiffNum);
    }
}