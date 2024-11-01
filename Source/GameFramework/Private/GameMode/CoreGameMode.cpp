// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "CoreGameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "CorePlayerController.h"
#include "CoreCharacter.h"
#include "UE4LogImpl.h"
#include "GameSystemManager.h"
#include "PlayerSystem.h"
#include "PlayerComponent.h"
#include "CoreCharacterStateBase.h"
#include "UnitSetting.h"
#include "ConfigTableCache.h"
#include "UnitInfoConfigTableRow.h"

void ACoreGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) {
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	if (!ErrorMessage.IsEmpty()) {
		return;
	}
}

APlayerController* ACoreGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) {
	FString RoleID = UGameplayStatics::ParseOption(Options, TEXT("RoleID"));
	FString UnitIDStr = UGameplayStatics::ParseOption(Options, TEXT("UnitID"));

	if (RoleID.IsEmpty()) {
		RoleID = FGuid::NewGuid().ToString();
	}
	if (UnitIDStr.IsEmpty()) {
		UnitIDStr = FString::FromInt(DebugStartTemplate.UnitID);
	}

	UE_LOG(GameCore, Display, TEXT("玩家登入，RoleID:%s"), *RoleID, *UnitIDStr);

	auto PlayerController =  Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	if (PlayerController) {
		auto PlayerState = PlayerController->GetPlayerState<ACoreCharacterStateBase>();
		if (PlayerState) {
			auto PlayerComponent = Cast<UPlayerComponent>(PlayerState->GetComponentByClass(UPlayerComponent::StaticClass()));
			if (PlayerComponent) {
				PlayerComponent->RoleID = RoleID;
				int UnitID = FCString::Atoi(*UnitIDStr);
				PlayerComponent->UnitID = UnitID;
			}
		}
	}
	return PlayerController;
}

void ACoreGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
}

void ACoreGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);
}

UClass* ACoreGameMode::GetDefaultPawnClassForController_Implementation(AController* InController) {
	auto PlayerController = Cast<ACorePlayerController>(InController);
	if (PlayerController) {
		auto PlayerState = PlayerController->GetPlayerState<ACoreCharacterStateBase>();
		if (PlayerState) {
			auto PlayerComponent = Cast<UPlayerComponent>(PlayerState->GetComponentByClass(UPlayerComponent::StaticClass()));
			if (PlayerComponent) {
				if (PlayerComponent->UnitID != 0) {
					const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
					auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
					if (UnitDataTable) {
						auto FindTemplate = (FUnitInfoConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(UnitDataTable, PlayerComponent->UnitID);
						if (FindTemplate && !FindTemplate->CharacterClass.GetLongPackageName().IsEmpty()) {
							auto CharacterClass = FindTemplate->CharacterClass.LoadSynchronous();
							if (CharacterClass) {
								return CharacterClass;
							}
						}
					}
				}
			}
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}