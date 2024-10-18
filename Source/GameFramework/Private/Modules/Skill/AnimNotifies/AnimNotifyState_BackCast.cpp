// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimNotifyState_BackCast.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Modules/Skill/SkillBlueprintLibrary.h"

void UAnimNotifyState_BackCast::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    UAnimMontage* AnimMontage = Cast<UAnimMontage>(Animation);
    if (!AnimMontage) {
        return;
    }
    if (!IsValid(MeshComp)) {
        return;
    }
    auto AbilitySystemComponent = UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
    if (!AbilitySystemComponent) {
        return;
    }
    auto Ability = USkillBlueprintLibrary::GetActiveAbilityWithCurrentMontage(AbilitySystemComponent, AnimMontage);
    if (Ability) {
        Ability->SetShouldBlockOtherAbilities(false);
    }
}

void UAnimNotifyState_BackCast::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    UAnimMontage* AnimMontage = Cast<UAnimMontage>(Animation);
    if (!AnimMontage) {
        return;
    }
    if (!IsValid(MeshComp)) {
        return;
    }
    auto AbilitySystemComponent = UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
    if (!AbilitySystemComponent) {
        return;
    }
    auto Ability = USkillBlueprintLibrary::GetActiveAbilityWithCurrentMontage(AbilitySystemComponent, AnimMontage);
    if (Ability) {
        Ability->SetShouldBlockOtherAbilities(true);
    }
}