#include "ConditionProgressLifeCycleManager.h"
#include "CoreConditionProgress.h"

class UCoreConditionProgress* UConditionProgressLifeCycleManager::MallocProgress(AActor* ProgressOwner, class UCoreCondition* Condition) {
    auto ConditionProgress = NewObject<UCoreConditionProgress>(ProgressOwner, Condition->ProgressClass);
    if (ConditionProgress) {
        ConditionProgress->Condition = Condition;
        ConditionProgress->ProgressOwner = ProgressOwner;
    }
    return ConditionProgress;
}

void UConditionProgressLifeCycleManager::FreeProgress(class UCoreConditionProgress* Progress) {

}