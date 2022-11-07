// Fill out your copyright notice in the Description page of Project Settings.

#include "GameArea.h"
#include "GameSystemManager.h"
#include "EventSystem.h"
#include "EnterAreaEvent.h"
#include "ExitAreaEvent.h"
#include "CoreCharacter.h"
#include "CoreGameInstance.h"
#include "Components/SceneComponent.h"
#include "UE4LogImpl.h"

AGameArea::AGameArea(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	bReplicates = true;
}

void AGameArea::OnEnter(ACoreCharacter* character) {
	if (character != nullptr) {
        auto gameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
		if (gameInstance != nullptr) {
            UE_LOG(GameCore, Log, TEXT("实体Id[%s] 进入区域[%s]"), *character->GetEntityID(), *AreaName);

			UEnterAreaEvent* enterAreaEvent = NewObject<UEnterAreaEvent>();
			enterAreaEvent->Character = character;
			enterAreaEvent->AreaName = AreaName;
			gameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(enterAreaEvent);
		}
	}
}

void AGameArea::OnExit(ACoreCharacter* character) {
	if (character != nullptr) {
        auto gameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
		if (gameInstance != nullptr) {
            UE_LOG(GameCore, Log, TEXT("实体Id[%s] 离开区域[%s]"), *character->GetEntityID(), *AreaName);

			UExitAreaEvent* exitAreaEvent = NewObject<UExitAreaEvent>();
			exitAreaEvent->Character = character;
			exitAreaEvent->AreaName = AreaName;
			gameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(exitAreaEvent);
		}
	}
}




