#include "CoreAbilityComboCheckerGlobal.h"
#include "CoreAbility.h"
#include "GameFrameworkUtils.h"
#include "AnimNotifyState_ComboEnable.h"
#include "CoreAbilitySystemComponent.h"

bool UCoreAbilityComboChecker_TriggerWayTag::CanComboExecute_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) {
	return MatchTriggerTag == TriggerWayTag;
}

bool UCoreAbilityComboChecker_ComboEnable::CanComboExecute_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) {
	auto MeshComponent = Ability->GetOwningComponentFromActorInfo();
	return UGameFrameworkUtils::GetAnyActiveAnimNotifyStateByClass(MeshComponent, UAnimNotifyState_ComboEnable::StaticClass()) != nullptr;
}

bool UCoreAbilityComboChecker_JumpAbilityValid::CanComboExecute_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) {
	if (SectionConfig.JumpAbility.Get()) {
		TArray<UGameplayAbility*> OutAbilities;
		AbilityComponent->GetActiveAbilitiesWithClass(SectionConfig.JumpAbility->GetClass(), OutAbilities);
		return OutAbilities.Num() > 0;
	}
	return true;
}

bool UCoreAbilityComboChecker_JumpSectionValid::CanComboExecute_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, const FComboSectionConfig& SectionConfig, FGameplayTag TriggerWayTag) {
	auto MeshComponent = Ability->GetOwningComponentFromActorInfo();
	auto AnimInstance = MeshComponent->GetAnimInstance();
	auto ActiveMontage = AnimInstance->GetCurrentActiveMontage();
	return ActiveMontage->IsValidSectionName(SectionConfig.JumpSection);
}