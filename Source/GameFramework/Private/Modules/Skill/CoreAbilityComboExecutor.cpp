#include "CoreAbilityComboExecutor.h"
#include "CoreAbility.h"
#include "CoreAbilitySystemComponent.h"

void UCoreAbilityComboExecutor::ExecuteCombo_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) {
	
}

void UCoreAbilityComboExecutor_Default::ExecuteCombo_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) {
	if (SectionConfig.JumpAbility.Get()) {
		Ability->CallEndAbility();
		AbilityComponent->TryActivateAbilityByClass(SectionConfig.JumpAbility);
		auto AbilitySpec = AbilityComponent->FindAbilitySpecFromClass(SectionConfig.JumpAbility);
		if (AbilitySpec) {
			TArray<UGameplayAbility*> ActivateAbilities = AbilitySpec->GetAbilityInstances();
			if (ActivateAbilities.Num() == 0) {
				return;
			}
			auto JumpAbility = Cast<UCoreAbility>(ActivateAbilities[0]);
			if (!JumpAbility) {
				return;
			}
			JumpAbility->NotifyComboAbility(AbilityComponent, FName());
		}
	}
	else {
		auto MeshComponent = Ability->GetOwningComponentFromActorInfo();
		auto AnimInstance = MeshComponent->GetAnimInstance();
		auto Montage = AnimInstance->GetCurrentActiveMontage();
		AnimInstance->Montage_JumpToSection(SectionConfig.JumpSection, Montage);
		Ability->NotifyComboAbility(AbilityComponent, SectionConfig.JumpSection);
	}
}