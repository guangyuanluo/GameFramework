// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Conditions/InputIDPressedTimeCondition.h"
#include "Modules/Skill/CoreAbilitySystemComponent.h"

UInputIDPressedTimeCondition::UInputIDPressedTimeCondition(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    ProgressClass = UInputIDPressedTimeConditionProgress::StaticClass();
}

EConditionSatisfy UInputIDPressedTimeConditionProgress::GetSatisfy() {
    auto AbilitySystemComponent = GetAbilitySystemComponent();
    auto ThisCondition = Cast<UInputIDPressedTimeCondition>(Condition);
    if (AbilitySystemComponent->GetInputPressTime(ThisCondition->InputID) >= ThisCondition->InputTimeLimit) {
        return EConditionSatisfy::ECS_Satisfy;
    }
    return EConditionSatisfy::ECS_NotSatisfy;
}