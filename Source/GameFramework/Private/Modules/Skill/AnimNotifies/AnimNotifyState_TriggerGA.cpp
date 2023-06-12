// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimNotifyState_TriggerGA.h"
#include "CoreCharacter.h"
#include "CoreAbilitySystemComponent.h"

void UAnimNotifyState_TriggerGA::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
    if (!TriggerGA) {
        return;
    }
    ACoreCharacter* Character = Cast<ACoreCharacter>(MeshComp->GetOwner());
    if (!Character) {
        return;
    }
    auto ABS = Character->GetAbilitySystemComponent();
    if (!ABS) {
        return;
    }
    ABS->TryActivateAbilityByClass(TriggerGA);
}