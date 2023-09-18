// Fill out your copyright notice in the Description page of Project Settings.

#include "CurrentComboSectionLimitCondition.h"
#include "GameFrameworkUtils.h"
#include "AnimNotifyState_ComboEnable.h"
#include "CoreAbilitySystemComponent.h"
#include "SkillBlueprintLibrary.h"
#include "SkillComboCacheComponent.h"

UCurrentComboSectionLimitCondition::UCurrentComboSectionLimitCondition(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    ProgressClass = UCurrentComboSectionLimitConditionProgress::StaticClass();
}

bool UCurrentComboSectionLimitConditionProgress::IsComplete_Implementation(bool& IsValid) {
    IsValid = false;
    auto AbilitySystemComponent = GetAbilitySystemComponent();
    auto SkillComboCacheComponent = USkillBlueprintLibrary::GetSkillComboCacheComponent(AbilitySystemComponent);
    if (!SkillComboCacheComponent) {
        return false;
    }
    IsValid = true;

    auto ThisCondition = Cast<UCurrentComboSectionLimitCondition>(Condition);
    return SkillComboCacheComponent->GetCurrentSectionName() == ThisCondition->SectionName;
}