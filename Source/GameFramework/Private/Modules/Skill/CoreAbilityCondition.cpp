#include "Modules/Skill/CoreAbilityCondition.h"
#include "PlayerState/CoreCharacterStateBase.h"

class UCoreAbility* UCoreAbilityConditionProgress::GetConditionAbility() const {
    return OwnerAbility.Get();
}