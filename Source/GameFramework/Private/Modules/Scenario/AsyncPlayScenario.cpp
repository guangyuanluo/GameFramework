// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Scenario/AsyncPlayScenario.h"
#include "GameInstance/CoreGameInstance.h"
#include "Base/ECS/GameSystemManager.h"
#include "Modules/Scenario/ScenarioSystem.h"
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