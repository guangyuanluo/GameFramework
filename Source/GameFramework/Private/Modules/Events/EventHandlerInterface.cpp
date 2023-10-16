#include "EventHandlerInterface.h"

TArray<TSubclassOf<class UGameEventBase>> IEventHandlerInterface::GetHandleEventTypes() {
    return K2_GetHandleEventTypes();
}

void IEventHandlerInterface::OnEvent(UCoreGameInstance* GameInstance, UGameEventBase* HandleEvent) {
    K2_OnEvent(GameInstance, HandleEvent);
}