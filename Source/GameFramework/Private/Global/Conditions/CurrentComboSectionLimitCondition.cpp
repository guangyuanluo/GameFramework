// Fill out your copyright notice in the Description page of Project Settings.

#include "CurrentComboSectionLimitCondition.h"
#include "GameFrameworkUtils.h"
#include "AnimNotifyState_ComboEnable.h"
#include "CoreAbilitySystemComponent.h"

UCurrentComboSectionLimitCondition::UCurrentComboSectionLimitCondition(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    ProgressClass = UCurrentComboSectionLimitConditionProgress::StaticClass();
}

bool UCurrentComboSectionLimitConditionProgress::IsComplete_Implementation(bool& IsValid) {
    IsValid = false;
    auto AbilitySystemComponent = GetAbilitySystemComponent();
    auto MeshComponent = AbilitySystemComponent->AbilityActorInfo->SkeletalMeshComponent.Get();
    if (!MeshComponent) {
        return false;
    }
    IsValid = true;

    auto ThisCondition = Cast<UCurrentComboSectionLimitCondition>(Condition);
    return AbilitySystemComponent->GetCurrentMontageSectionName() == ThisCondition->SectionName && UGameFrameworkUtils::GetAnyActiveAnimNotifyStateByClass(MeshComponent, UAnimNotifyState_ComboEnable::StaticClass()) != nullptr;
}