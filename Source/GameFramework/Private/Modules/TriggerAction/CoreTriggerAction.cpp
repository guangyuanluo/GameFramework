// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreTriggerAction.h"
#include "CoreAbility.h"

void UCoreTriggerAction::OnExecute_Implementation(const FGameplayEventData& EventData) const {
    
}

class UCoreAbility* UCoreTriggerAction::GetExecuteAbility(const FGameplayEventData& EventData) const {
    if (!EventData.ContextHandle.IsValid()) {
        return nullptr;
    }
    auto Ability = Cast<UCoreAbility>(EventData.ContextHandle.GetAbilityInstance_NotReplicated());
    if (!Ability) {
        return nullptr;
    }
    return const_cast<UCoreAbility*>(Ability);
}