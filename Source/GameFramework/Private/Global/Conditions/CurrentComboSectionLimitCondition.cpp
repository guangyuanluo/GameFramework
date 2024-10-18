// Fill out your copyright notice in the Description page of Project Settings.

#include "CurrentComboSectionLimitCondition.h"
#include "GameFrameworkUtils.h"
#include "AnimNotifyState_ComboEnable.h"
#include "CoreAbilitySystemComponent.h"
#include "SkillBlueprintLibrary.h"
#include "CoreCharacter.h"
#include "AnimNotifyState_BackCast.h"

UCurrentComboSectionLimitCondition::UCurrentComboSectionLimitCondition(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    ProgressClass = UCurrentComboSectionLimitConditionProgress::StaticClass();
}

bool UCurrentComboSectionLimitConditionProgress::IsComplete(bool& IsValid) {
    IsValid = false;
    auto AbilitySystemComponent = GetAbilitySystemComponent();
    auto Character = Cast<ACoreCharacter>(AbilitySystemComponent->GetAvatarActor());
    if (!Character) {
        return false;
    }
    auto CharMesh = Character->GetMesh();
    if (!CharMesh) {
        return false;
    }
    IsValid = true;
    auto ThisCondition = Cast<UCurrentComboSectionLimitCondition>(Condition);
    auto ActiveAbility = AbilitySystemComponent->GetAnimatingAbility();
    if (!ActiveAbility) {
        return false;
    }
    if (!ActiveAbility->AbilityTags.HasTag(ThisCondition->ComboTag)) {
        return false;
    }

    auto AnimInstance = CharMesh->GetAnimInstance();
    auto CurrentMontage = ActiveAbility->GetCurrentMontage();

    bool bInComboEnable = false;
    bool bAfterBackcast = false;
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
        return false;
    }

    auto FindMontageInstance = AnimInstance->MontageInstances[FindMontageIndex];
    auto CurrentTrackPosition = FindMontageInstance->GetPosition();
    for (int32 Index = 0; Index < FindMontageInstance->Montage->Notifies.Num(); ++Index) {
        FAnimNotifyEvent& NotifyEvent = FindMontageInstance->Montage->Notifies[Index];
        if (NotifyEvent.NotifyStateClass) {
            if (!bInComboEnable) {
                if (NotifyEvent.NotifyStateClass->GetClass()->IsChildOf(UAnimNotifyState_ComboEnable::StaticClass())) {
                    //不在连招允许时间
                    const float NotifyStartTime = NotifyEvent.GetTriggerTime();
                    const float NotifyEndTime = NotifyEvent.GetEndTriggerTime();
                    if ((CurrentTrackPosition > NotifyStartTime) && (CurrentTrackPosition <= NotifyEndTime)) {
                        bInComboEnable = true;
                    }
                }
            }
            if (!bAfterBackcast) {
                if (NotifyEvent.NotifyStateClass->GetClass()->IsChildOf(UAnimNotifyState_BackCast::StaticClass())) {
                    //不在后摇之后
                    const float NotifyStartTime = NotifyEvent.GetTriggerTime();
                    if (CurrentTrackPosition > NotifyStartTime) {
                        bAfterBackcast = true;
                    }
                }
            }
            if (bInComboEnable && bAfterBackcast) {
                break;
            }
        }
    }

    if (bInComboEnable && bAfterBackcast) {
        return true;
    }

    return false;
}