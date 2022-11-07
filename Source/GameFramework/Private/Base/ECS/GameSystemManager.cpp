// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSystemManager.h"
#include "UE4LogImpl.h"
#include "GameSystemFeature.h"

void UGameSystemManager::Initialize(UCoreGameInstance* InGameInstance) {
	GameInstance = InGameInstance;

    AddGameSystemFeature(DefaultFeature);
}

void UGameSystemManager::Uninitialize() {
    RemoveGameSystemFeature(DefaultFeature);

    SystemMap.Empty();

    GameInstance = nullptr;
}

USystemBase* UGameSystemManager::K2_GetSystemByClass(TSubclassOf<USystemBase> SystemClass) const {
    auto FindPtr = SystemMap.Find(SystemClass);
    if (FindPtr) {
        return FindPtr->SystemInstance;
    }
    else {
        return nullptr;
    }
}

void UGameSystemManager::AddGameSystemFeature(TSubclassOf<UGameSystemFeature> SystemFeatureClass) {
    if (!SystemFeatureClass) {
        return;
    }
    auto GameSystemFeature = Cast<UGameSystemFeature>(SystemFeatureClass->GetDefaultObject());
    if (!GameSystemFeature) {
        return;
    }
    auto& FeatureSystemClasses = GameSystemFeature->SystemClasses;
    for (int Index = 0; Index < FeatureSystemClasses.Num(); ++Index) {
        UClass* SystemClass = FeatureSystemClasses[Index];
        if (SystemClass) {
            auto SystemCDO = Cast<USystemBase>(SystemClass->GetDefaultObject());
            auto SystemType = SystemCDO->GetSystemType();
            if (SystemType == ESystemType::E_Both) {
                FGameSystemInfo& SystemInfo = SystemMap.FindOrAdd(SystemClass);
                if (SystemInfo.SystemInstance == nullptr) {
                    auto System = NewObject<USystemBase>(this, SystemClass);
                    System->AddToRoot();
                    SystemInfo.SystemInstance = System;
                }
                ++SystemInfo.RefCount;
            }
        }
        else {
            UE_LOG(GameCore, Error, TEXT("GameSystemFeature存在空的SystemClass，请检查配置"));
        }
    }

    for (int Index = 0; Index < FeatureSystemClasses.Num(); ++Index) {
        UClass* SystemClass = FeatureSystemClasses[Index];
        if (SystemClass) {
            auto& SystemInfo = SystemMap[SystemClass];
            if (!SystemInfo.IsInitialize) {
                SystemInfo.IsInitialize = true;
                UE_LOG(GameCore, Log, TEXT("InitSystem:%s"), *SystemClass->GetName());
                SystemInfo.SystemInstance->Initialize(GameInstance);
            }
        }
    }
}

void UGameSystemManager::RemoveGameSystemFeature(TSubclassOf<UGameSystemFeature> SystemFeatureClass) {
    if (!SystemFeatureClass) {
        return;
    }
    auto GameSystemFeature = Cast<UGameSystemFeature>(SystemFeatureClass->GetDefaultObject());
    if (!GameSystemFeature) {
        return;
    }
    auto& FeatureSystemClasses = GameSystemFeature->SystemClasses;
    for (int Index = 0; Index < FeatureSystemClasses.Num(); ++Index) {
        UClass* SystemClass = FeatureSystemClasses[Index];
        auto SystemPtr = SystemMap.Find(SystemClass);
        if (SystemPtr) {
            --SystemPtr->RefCount;
            if (SystemPtr->RefCount <= 0) {
                SystemPtr->SystemInstance->Uninitialize();
            }
        }
    }
    for (int Index = 0; Index < FeatureSystemClasses.Num(); ++Index) {
        UClass* SystemClass = FeatureSystemClasses[Index];
        if (SystemClass) {
            auto& SystemInfo = SystemMap[SystemClass];
            auto SystemPtr = SystemMap.Find(SystemClass);
            if (SystemPtr->RefCount <= 0) {
                SystemPtr->SystemInstance->RemoveFromRoot();
                SystemMap.Remove(SystemClass);
            }
        }
    }
}