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

void AGameArea::OnEnter(ACoreCharacter* Character) {
	if (Character != nullptr) {
        auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
		if (!GameInstance || !GameInstance->GameSystemManager) {
			return;
		}
		auto EventSystem = GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
		if (!EventSystem) {
			return;
		}
        UE_LOG(GameCore, Log, TEXT("实体Id[%s] 进入区域[%s]"), *Character->GetEntityID(), *AreaName);

		UEnterAreaEvent* EnterAreaEvent = NewObject<UEnterAreaEvent>();
		EnterAreaEvent->Character = Character;
		EnterAreaEvent->AreaName = AreaName;
		EventSystem->PushEvent(EnterAreaEvent);
	}
}

void AGameArea::OnExit(ACoreCharacter* Character) {
	if (Character != nullptr) {
        auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
		if (!GameInstance || !GameInstance->GameSystemManager) {
			return;
		}
		auto EventSystem = GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();
		if (!EventSystem) {
			return;
		}

        UE_LOG(GameCore, Log, TEXT("实体Id[%s] 离开区域[%s]"), *Character->GetEntityID(), *AreaName);

		UExitAreaEvent* ExitAreaEvent = NewObject<UExitAreaEvent>();
		ExitAreaEvent->Character = Character;
		ExitAreaEvent->AreaName = AreaName;
		EventSystem->PushEvent(ExitAreaEvent);
	}
}




