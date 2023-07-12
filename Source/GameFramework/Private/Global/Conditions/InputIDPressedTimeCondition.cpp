// Fill out your copyright notice in the Description page of Project Settings.

#include "InputIDPressedTimeCondition.h"
#include "CoreAbilitySystemComponent.h"

UInputIDPressedTimeCondition::UInputIDPressedTimeCondition(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    ProgressClass = UInputIDPressedTimeConditionProgress::StaticClass();
}

bool UInputIDPressedTimeConditionProgress::IsComplete_Implementation(bool& IsValid) {
    IsValid = false;
    auto AbilitySystemComponent = GetAbilitySystemComponent();
    IsValid = true;
    auto ThisCondition = Cast<UInputIDPressedTimeCondition>(Condition);
    return AbilitySystemComponent->GetInputPressTime(ThisCondition->InputID) >= ThisCondition->InputTimeLimit;
}