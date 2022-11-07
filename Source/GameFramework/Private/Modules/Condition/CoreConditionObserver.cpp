#include "CoreConditionObserver.h"
#include "CoreGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameSystemManager.h"
#include "ConditionSystem.h"

void ICoreConditionObserver::Unfollow() {
    auto Object = _getUObject();
    auto GameInstance = Cast<UCoreGameInstance>(UGameplayStatics::GetGameInstance(Object));
    if (GameInstance) {
        UConditionSystem* ConditionSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionSystem>();
        if (ConditionSystem) {
            ConditionSystem->UnfollowConditions(Object);
        }
    }
}