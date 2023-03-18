#include "CoreAbilityConditionGlobal.h"
#include "CoreAbility.h"
#include "GameFrameworkUtils.h"
#include "AnimNotifyState_ComboEnable.h"
#include "CoreAbilitySystemComponent.h"

void UCoreAbilityCondition_CurrentComboSectionLimit::DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, bool& bValid, bool& bSatisfy) {
    auto MeshComponent = Ability->GetOwningComponentFromActorInfo();
    if (MeshComponent) {
        bValid = true;

        bSatisfy = AbilityComponent->GetCurrentMontageSectionName() == SectionName && UGameFrameworkUtils::GetAnyActiveAnimNotifyStateByClass(MeshComponent, UAnimNotifyState_ComboEnable::StaticClass()) != nullptr;
    }
    else {
        bValid = false;
    }
}

void UCoreAbilityCondition_InputTime::DoesSatisfy_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, UCoreAbility* Ability, bool& bValid, bool& bSatisfy) {
    auto FindSpec = AbilityComponent->FindAbilitySpecFromClass(Ability->GetClass());
    if (FindSpec) {
        bValid = true;
        bSatisfy = AbilityComponent->GetInputPressTime(FindSpec->InputID) >= InputTimeLimit;
    }
    else {
        bValid = false;
    }
}