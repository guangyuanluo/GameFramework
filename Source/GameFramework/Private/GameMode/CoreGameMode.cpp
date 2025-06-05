// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/CoreGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/CoreGameInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PlayerController/CorePlayerController.h"
#include "Character/CoreCharacter.h"
#include "Base/Log/UE4LogImpl.h"
#include "Base/ECS/GameSystemManager.h"
#include "Modules/Player/PlayerSystem.h"
#include "Modules/Player/PlayerComponent.h"
#include "PlayerState/CoreCharacterStateBase.h"
#include "Modules/Unit/UnitSetting.h"
#include "Base/ConfigTable/ConfigTableCache.h"
#include "Modules/Unit/UnitInfoConfigTableRow.h"

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

	UE_LOG(GameCore, Display, TEXT("玩家登入，RoleID:%s %s"), *RoleID, *UnitIDStr);

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

APawn* ACoreGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) {
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	SpawnInfo.bDeferConstruction = true;

	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);

	APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
	if (!ResultPawn)
	{
		UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
	}
	if (NewPlayer->PlayerState) {
		ResultPawn->SetPlayerState(NewPlayer->PlayerState);
	}
	UGameplayStatics::FinishSpawningActor(ResultPawn, SpawnTransform);
	return ResultPawn;
}