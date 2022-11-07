// Fill out your copyright notice in the Description page of Project Settings.

#include "SystemBase.h"

void USystemBase::Initialize(UCoreGameInstance* InGameInstance) {
	GameInstance = InGameInstance;

    PostInitialize();
}

void USystemBase::Uninitialize() {
    PreUninitialize();

    GameInstance = nullptr;
}

ESystemType USystemBase::GetSystemType() const {
    return SystemType;
}