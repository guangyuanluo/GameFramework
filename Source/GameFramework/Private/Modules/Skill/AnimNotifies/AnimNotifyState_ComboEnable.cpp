// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimNotifyState_ComboEnable.h"
#include "SkillBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "SkillComboCacheComponent.h"

void UAnimNotifyState_ComboEnable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	auto Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) {
		return;
	}
	auto ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);
	if (!ASC) {
		return;
	}
	auto SkillComboCacheComponent = USkillBlueprintLibrary::GetSkillComboCacheComponent(ASC);
	if (!SkillComboCacheComponent) {
		return;
	}
	SkillComboCacheComponent->SetCurrentSectionName(ASC->GetCurrentMontageSectionName());
}

void UAnimNotifyState_ComboEnable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	auto Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) {
		return;
	}
	auto ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);
	if (!ASC) {
		return;
	}
	auto SkillComboCacheComponent = USkillBlueprintLibrary::GetSkillComboCacheComponent(ASC);
	if (!SkillComboCacheComponent) {
		return;
	}
	SkillComboCacheComponent->SetCurrentSectionName(FName());
}