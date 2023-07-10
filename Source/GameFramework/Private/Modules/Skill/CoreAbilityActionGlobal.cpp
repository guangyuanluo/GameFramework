#include "CoreAbilityActionGlobal.h"
#include "CoreAbility.h"
#include "CoreAbilitySystemComponent.h"

void UAbilityTriggerAction_ChangeRestCounter::OnExecute_Implementation(const FGameplayEventData& EventData) const {
    if (!EventData.ContextHandle.IsValid()) {
        return;
    }
    auto AbilityConst = Cast<UCoreAbility>(EventData.ContextHandle.GetAbilityInstance_NotReplicated());
    if (!AbilityConst) {
        return;
    }
    auto Ability = const_cast<UCoreAbility*>(AbilityConst);
    auto AbilitySystemComponent = Cast<UCoreAbilitySystemComponent>(Ability->GetAbilitySystemComponentFromActorInfo());
    AbilitySystemComponent->ChangeAbilityRestCounter(Ability, ChangeCounter);
}