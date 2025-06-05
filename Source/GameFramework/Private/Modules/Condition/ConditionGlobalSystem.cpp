#include "Modules/Condition/ConditionGlobalSystem.h"
#include "Modules/Condition/ConditionProgressLifeCycleManager.h"

class UCoreConditionProgress* UConditionGlobalSystem::MallocProgress(AActor* ProgressOwner, class UCoreCondition* Condition) {
    if (!ProgressLifeCycleManagerCDO) {
        TSubclassOf<UConditionProgressLifeCycleManager> ConditionProgressLifeCycleManagerClass = ProgressLifeCycleManager.Get() ? ProgressLifeCycleManager.Get() : UConditionProgressLifeCycleManager::StaticClass();
        ProgressLifeCycleManagerCDO = ConditionProgressLifeCycleManagerClass->GetDefaultObject<UConditionProgressLifeCycleManager>();
    }
    return ProgressLifeCycleManagerCDO->MallocProgress(ProgressOwner, Condition);
}

void UConditionGlobalSystem::FreeProgress(class UCoreConditionProgress* Progress) {
    if (!ProgressLifeCycleManagerCDO) {
        TSubclassOf<UConditionProgressLifeCycleManager> ConditionProgressLifeCycleManagerClass = ProgressLifeCycleManager.Get() ? ProgressLifeCycleManager.Get() : UConditionProgressLifeCycleManager::StaticClass();
        ProgressLifeCycleManagerCDO = ConditionProgressLifeCycleManagerClass->GetDefaultObject<UConditionProgressLifeCycleManager>();
    }
    ProgressLifeCycleManagerCDO->FreeProgress(Progress);
}