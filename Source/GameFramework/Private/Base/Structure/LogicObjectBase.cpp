#include "LogicObjectBase.h"

UWorld* ULogicObjectBase::GetWorld() const {
    if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
        return GetOuter()->GetWorld();
    else
        return nullptr;
}