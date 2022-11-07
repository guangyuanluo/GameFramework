// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEntity.h"

// Add default functionality here for any IGameEntity functions that are not pure virtual.
const FString& IGameEntity::GetEntityID() {
    return EntityID_Implementation();
}

void IGameEntity::SetEntityID(const FString& EntityID) {
    EntityID_Implementation() = EntityID;
}

const FTransform& IGameEntity::GetTransform() const {
    return FTransform::Identity;
}

void IGameEntity::SetTransform(const FTransform& NewTransform) {

}