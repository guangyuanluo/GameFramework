#include "CoreAbilityCondition.h"

class UCoreAbility* UCoreAbilityConditionProgress::GetConditionAbility() const {
    return OwnerAbility.Get();
}