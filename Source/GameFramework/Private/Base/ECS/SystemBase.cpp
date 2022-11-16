// Fill out your copyright notice in the Description page of Project Settings.

#include "SystemBase.h"
#include "CoreGameInstance.h"

void USystemBase::Initialize(UCoreGameInstance* InGameInstance) {
	GameInstance = InGameInstance;

    PostInitialize();
}

void USystemBase::Uninitialize() {
    PreUninitialize();

    GameInstance = nullptr;
}

void USystemBase::PostInitialize_Implementation() {

}

void USystemBase::PreUninitialize_Implementation() {

}

void USystemBase::OnTick_Implementation(float DeltaTime) {

}

ESystemType USystemBase::GetSystemType() const {
    return SystemType;
}

void USystemBase::Tick(float DeltaTime) {
    OnTick(DeltaTime);
}

TStatId USystemBase::GetStatId() const {
    RETURN_QUICK_DECLARE_CYCLE_STAT(SystemBase, STATGROUP_Tickables);
}

ETickableTickType USystemBase::GetTickableTickType() const {
    if (CanTick) {
        return HasAnyFlags(RF_ClassDefaultObject) ? ETickableTickType::Never : ETickableTickType::Always;
    }
    else {
        return ETickableTickType::Never;
    }
}

bool USystemBase::IsTickable() const {
    return CanTick;
}

UWorld* USystemBase::GetWorld() const {
    if (GameInstance) {
        return GameInstance->GetWorld();
    }
    else {
        return nullptr;
    }
}