#include "CoreAbilityConditionGlobal.h"
#include "CoreAbility.h"
#include "GameFrameworkUtils.h"
#include "AnimNotifyState_ComboEnable.h"
#include "CoreAbilitySystemComponent.h"
#include "CoreCharacterStateBase.h"
#include "GameFramework/Character.h"

UCoreAbilityCondition_CurrentComboSectionLimit::UCoreAbilityCondition_CurrentComboSectionLimit(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    ProgressClass = UCoreAbilityConditionProgress_CurrentComboSectionLimit::StaticClass();
}

bool UCoreAbilityConditionProgress_CurrentComboSectionLimit::IsComplete_Implementation(bool& IsValid) {
    IsValid = false;
    auto Ability = GetConditionAbility();
    auto AbilityComponent = Ability->GetAbilitySystemComponentFromActorInfo();
    auto MeshComponent = Ability->GetOwningComponentFromActorInfo();
    if (MeshComponent) {
        IsValid = true;

        auto ThisCondition = Cast<UCoreAbilityCondition_CurrentComboSectionLimit>(Condition);

        return AbilityComponent->GetCurrentMontageSectionName() == ThisCondition->SectionName && UGameFrameworkUtils::GetAnyActiveAnimNotifyStateByClass(MeshComponent, UAnimNotifyState_ComboEnable::StaticClass()) != nullptr;
    }
    else {
        IsValid = false;
    }
    return false;
}

UCoreAbilityCondition_InputTime::UCoreAbilityCondition_InputTime(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    ProgressClass = UCoreAbilityConditionProgress_InputTime::StaticClass();
}

bool UCoreAbilityConditionProgress_InputTime::IsComplete_Implementation(bool& IsValid) {
    IsValid = false;
    auto Ability = GetConditionAbility();
    auto AbilityComponent = Cast<UCoreAbilitySystemComponent>(Ability->GetAbilitySystemComponentFromActorInfo());
    auto FindSpec = AbilityComponent->FindAbilitySpecFromClass(Ability->GetClass());
    if (FindSpec) {
        IsValid = true;
        auto ThisCondition = Cast<UCoreAbilityCondition_InputTime>(Condition);
        return AbilityComponent->GetInputPressTime(FindSpec->InputID) >= ThisCondition->InputTimeLimit;
    }
    return false;
}