#include "CoreAbilityCondition.h"
#include "CoreCharacterStateBase.h"

class UCoreAbility* UCoreAbilityConditionProgress::GetConditionAbility() const {
    return OwnerAbility.Get();
}