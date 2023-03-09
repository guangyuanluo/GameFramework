#include "LogicObjectBase.h"

FLogicObjectLoadWorldScope::FLogicObjectLoadWorldScope() 
: LogicObjectBase(nullptr)
, WorldContext(nullptr) {

}

FLogicObjectLoadWorldScope::FLogicObjectLoadWorldScope(class ULogicObjectBase* InLogicObjectBase, class UObject* InWorldContext) 
: LogicObjectBase(InLogicObjectBase)
, WorldContext(InWorldContext) {
    Reload();
}

FLogicObjectLoadWorldScope::~FLogicObjectLoadWorldScope() {
    if (LogicObjectBase) {
        LogicObjectBase->LoadWorldContext(nullptr);
    }
}

void FLogicObjectLoadWorldScope::Reload() {
    if (LogicObjectBase) {
        LogicObjectBase->LoadWorldContext(WorldContext);
    }
}

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

UObject* ULogicObjectBase::GetWorldContextObject() const {
    return WorldContextObject;
}

void ULogicObjectUtils::LogicObjectLoadWorld(const FLogicObjectLoadWorldScope& Scope, class ULogicObjectBase* InLogicObjectBase, class UObject* InWorldContext) {
    const FLogicObjectLoadWorldScope* ConstScopePtr = &Scope;
    FLogicObjectLoadWorldScope* ScopePtr = const_cast<FLogicObjectLoadWorldScope*>(ConstScopePtr);
    ScopePtr->LogicObjectBase = InLogicObjectBase;
    ScopePtr->WorldContext = InWorldContext;

    ScopePtr->Reload();
}