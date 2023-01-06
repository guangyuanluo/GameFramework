#include "LogicObjectBase.h"

UWorld* ULogicObjectBase::GetWorld() const {
    if (WorldContextObject != nullptr) {
        return WorldContextObject->GetWorld();
    }
    if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
        return GetOuter()->GetWorld();
    else
        return nullptr;
}

void ULogicObjectBase::LoadWorldContext(UObject* InWorldContextObject) {
    WorldContextObject = InWorldContextObject;
}