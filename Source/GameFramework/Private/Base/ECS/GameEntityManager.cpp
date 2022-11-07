#include "GameEntityManager.h"

TScriptInterface<IGameEntity> UGameEntityManager::GetEntityById(const FString& EntityId) {
    auto findPtr = EntityMap.Find(EntityId);
    if (findPtr) {
        return *findPtr;
    }
    return nullptr;
}

void UGameEntityManager::OnEntityAdd(TScriptInterface<IGameEntity> Entity) {
    FString EntityID = Entity->GetEntityID();
    if (EntityID.IsEmpty()) {
        EntityID = FGuid::NewGuid().ToString();
        Entity->SetEntityID(EntityID);
    }

    EntityMap.Add(EntityID, Entity);
}

void UGameEntityManager::OnEntityRemove(TScriptInterface<IGameEntity> Entity) {
    FString EntityId = Entity->GetEntityID();
    EntityMap.Remove(EntityId);
}

void UGameEntityManager::OnEntityIDChanged(TScriptInterface<IGameEntity> Entity, const FString& OldID) {
    EntityMap.Remove(OldID);

    FString EntityID = Entity->GetEntityID();
    EntityMap.Add(EntityID, Entity);
}