#include "CoreAbilityConditionGlobal.h"
#include "CoreAbility.h"
#include "GameFrameworkUtils.h"
#include "AnimNotifyState_ComboEnable.h"
#include "CoreAbilitySystemComponent.h"

bool UCoreAbilityCondition_ComboEnable::DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability) {
	auto MeshComponent = Ability->GetOwningComponentFromActorInfo();
	return UGameFrameworkUtils::GetAnyActiveAnimNotifyStateByClass(MeshComponent, UAnimNotifyState_ComboEnable::StaticClass()) != nullptr;
}

bool UCoreAbilityCondition_ComboJumpAbilityValid::DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability) {
	auto CurrentSectoinConfig = AbilityComponent->GetCurrentCheckComboSection();
	check(CurrentSectoinConfig);
	if (CurrentSectoinConfig->JumpAbility.Get()) {
		TArray<UGameplayAbility*> OutAbilities;
		AbilityComponent->GetActiveAbilitiesWithClass(CurrentSectoinConfig->JumpAbility, OutAbilities);
		if (OutAbilities.Num() != 0) {
			return true;
		}
		return false;
	}
	else {
		return true;
	}
}

bool UCoreAbilityCondition_ComboJumpSectionValid::DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability) {
	auto CurrentSectoinConfig = AbilityComponent->GetCurrentCheckComboSection();
	check(CurrentSectoinConfig);
	if (CurrentSectoinConfig->JumpSection.IsNone()) {
		return true;
	}
	else {
		auto MeshComponent = Ability->GetOwningComponentFromActorInfo();
		auto AnimInstance = MeshComponent->GetAnimInstance();
		auto ActiveMontage = AnimInstance->GetCurrentActiveMontage();
		return ActiveMontage->IsValidSectionName(CurrentSectoinConfig->JumpSection);
	}
}

bool UCoreAbilityCondition_TriggerWayTag::DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability) {
    return MatchTriggerTag == AbilityComponent->GetCurrentTriggerWayTag();
}