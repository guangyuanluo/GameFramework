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
    auto AbilitySystemComponent = GetAbilitySystemComponent();
    auto MeshComponent = AbilitySystemComponent->AbilityActorInfo->SkeletalMeshComponent.Get();
    if (MeshComponent) {
        IsValid = true;

        auto ThisCondition = Cast<UCoreAbilityCondition_CurrentComboSectionLimit>(Condition);

        return AbilitySystemComponent->GetCurrentMontageSectionName() == ThisCondition->SectionName && UGameFrameworkUtils::GetAnyActiveAnimNotifyStateByClass(MeshComponent, UAnimNotifyState_ComboEnable::StaticClass()) != nullptr;
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
    auto AbilitySystemComponent = GetAbilitySystemComponent();
    IsValid = true;
    auto ThisCondition = Cast<UCoreAbilityCondition_InputTime>(Condition);
    return AbilitySystemComponent->GetInputPressTime(ThisCondition->InputID) >= ThisCondition->InputTimeLimit;
}

UCoreAbilityCondition_AbilityLimitCounter::UCoreAbilityCondition_AbilityLimitCounter(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    ProgressClass = UCoreAbilityConditionProgress_AbilityLimitCounter::StaticClass();
}

void UCoreAbilityConditionProgress_AbilityLimitCounter::OnInitialize_Implementation() {
    Super::OnInitialize_Implementation();

    auto ThisCondition = Cast<UCoreAbilityCondition_AbilityLimitCounter>(Condition);

    CurrentCounter = ThisCondition->LimitCounter;

    auto Ability = GetConditionAbility();
    auto AbilitySystemComponent = GetAbilitySystemComponent();
    AbilitySystemComponent->SetAbilityLimitCounter(Ability, ThisCondition->LimitCounter);
    AbilitySystemComponent->OnAbilityRestCounterUpdateDelegate.AddUObject(this, &UCoreAbilityConditionProgress_AbilityLimitCounter::OnAbilityRestCounterUpdate);
}

void UCoreAbilityConditionProgress_AbilityLimitCounter::OnUninitialize_Implementation() {
    Super::OnUninitialize_Implementation();
    
    auto Ability = GetConditionAbility();
    auto AbilitySystemComponent = GetAbilitySystemComponent();
    AbilitySystemComponent->RemoveAbilityLimitCounter(Ability);
    AbilitySystemComponent->OnAbilityRestCounterUpdateDelegate.RemoveAll(this);
}

bool UCoreAbilityConditionProgress_AbilityLimitCounter::IsComplete_Implementation(bool& IsValid) {
    IsValid = true;
    return CurrentCounter <= 0;
}

void UCoreAbilityConditionProgress_AbilityLimitCounter::OnAbilityRestCounterUpdate(class UCoreAbilitySystemComponent* AbilitySystemComponent, class UCoreAbility* Ability, int NowCounter) {
    CurrentCounter = NowCounter;
    if (CurrentCounter <= 0) {
        RefreshSatisfy();
    }
}