#include "CoreAbilityCondition.h"
#include "CoreCharacterStateBase.h"

class UCoreAbility* UCoreAbilityConditionProgress::GetConditionAbility() const {
    return OwnerAbility.Get();
}

class UCoreAbilitySystemComponent* UCoreAbilityConditionProgress::GetAbilitySystemComponent() const {
    auto CharacterState = Cast<ACoreCharacterStateBase>(ProgressOwner);
    if (CharacterState) {
        return Cast<UCoreAbilitySystemComponent>(CharacterState->GetAbilitySystemComponent());
    }
    return nullptr;
}