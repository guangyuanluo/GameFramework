// Copyright Epic Games, Inc. All Rights Reserved.

#include "AsyncPlayScenario.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "ScenarioSystem.h"
#include "Kismet/GameplayStatics.h"

UAsyncPlayScenario::UAsyncPlayScenario(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
    
}

UAsyncPlayScenario* UAsyncPlayScenario::PlayScenario(UObject* WorldContextObject, UScenarioComponent* ScenarioComponent, UScenario* InScenario) {
    auto GameInstance = Cast<UCoreGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (GameInstance) {
        return GameInstance->GameSystemManager->GetSystemByClass<UScenarioSystem>()->PlayScenario(ScenarioComponent, InScenario);
    }
    return nullptr;
}