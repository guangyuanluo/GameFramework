// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/Conditions/CurrentComboSectionLimitCondition.h"
#include "Utils/GameFrameworkUtils.h"
#include "Modules/Skill/AnimNotifies/AnimNotifyState_ComboEnable.h"
#include "Modules/Skill/CoreAbilitySystemComponent.h"
#include "Modules/Skill/SkillBlueprintLibrary.h"
#include "Character/CoreCharacter.h"
#include "Modules/Skill/AnimNotifies/AnimNotifyState_BackCast.h"

UCurrentComboSectionLimitCondition::UCurrentComboSectionLimitCondition(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    ProgressClass = UCurrentComboSectionLimitConditionProgress::StaticClass();
}

EConditionSatisfy UCurrentComboSectionLimitConditionProgress::GetSatisfy() {
    auto AbilitySystemComponent = GetAbilitySystemComponent();
    auto Character = Cast<ACoreCharacter>(AbilitySystemComponent->GetAvatarActor());
    if (!Character) {
        return EConditionSatisfy::ECS_Unknown;
    }
    auto CharMesh = Character->GetMesh();
    if (!CharMesh) {
        return EConditionSatisfy::ECS_Unknown;
    }
    auto ThisCondition = Cast<UCurrentComboSectionLimitCondition>(Condition);
    auto ActiveAbility = AbilitySystemComponent->GetAnimatingAbility();
    if (!ActiveAbility) {
        return EConditionSatisfy::ECS_NotSatisfy;
    }
    if (!ActiveAbility->GetAssetTags().HasTag(ThisCondition->ComboTag)) {
        return EConditionSatisfy::ECS_NotSatisfy;
    }

    auto AnimInstance = CharMesh->GetAnimInstance();
    auto CurrentMontage = ActiveAbility->GetCurrentMontage();

    int FindMontageIndex = -1;
    for (auto Index = 0; Index < AnimInstance->MontageInstances.Num(); ++Index) {
        auto MontageInstance = AnimInstance->MontageInstances[Index];
        if (!MontageInstance || MontageInstance->Montage != CurrentMontage) {
            continue;
        }
        FindMontageIndex = Index;
        break;
    }
    if (FindMontageIndex == -1) {
        return EConditionSatisfy::ECS_NotSatisfy;
    }

    auto FindMontageInstance = AnimInstance->MontageInstances[FindMontageIndex];
    auto CurrentTrackPosition = FindMontageInstance->GetPosition();
    for (int32 Index = 0; Index < FindMontageInstance->Montage->Notifies.Num(); ++Index) {
        FAnimNotifyEvent& NotifyEvent = FindMontageInstance->Montage->Notifies[Index];
        if (NotifyEvent.NotifyStateClass) {
            if (NotifyEvent.NotifyStateClass->GetClass()->IsChildOf(UAnimNotifyState_ComboEnable::StaticClass())) {
                // 检查是否在通知时间内
                const float NotifyStartTime = NotifyEvent.GetTriggerTime();
                const float NotifyEndTime = NotifyEvent.GetEndTriggerTime();
                if ((CurrentTrackPosition > NotifyStartTime) && (CurrentTrackPosition <= NotifyEndTime)) {
                    return EConditionSatisfy::ECS_Satisfy;
                }
            }
        }
    }

    return EConditionSatisfy::ECS_NotSatisfy;
}