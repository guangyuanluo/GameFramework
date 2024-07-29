// Fill out your copyright notice in the Description page of Project Settings.

#include "CorePlayerController.h"
#include "Engine/World.h"
#include "CoreGameInstance.h"
#include "CoreCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerSystem.h"
#include "UE4LogImpl.h"
#include "EventSystem.h"
#include "GameEntityManager.h"
#include "GameSystemManager.h"
#include "CoreSceneItem.h"
#include "Kismet/GameplayStatics.h"
#include "CoreCharacterStateBase.h"
#include "PlayerComponent.h"
#include "GMComponent.h"
#include "UnitSetting.h"
#include "ConfigTableCache.h"
#include "UnitInfoConfigTableRow.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/CorePlayerInput.h"

ACorePlayerController::ACorePlayerController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    GMComponent = CreateDefaultSubobject<UGMComponent>(TEXT("GMComponent"));

	OverridePlayerInputClass = UCorePlayerInput::StaticClass();
}

void ACorePlayerController::InitRoleId(const FString& RoleID)
{
    auto CorePlayerState = GetPlayerState<ACoreCharacterStateBase>();
    if (ensureMsgf(CorePlayerState, TEXT("CorePlayeState为空"))) {
        CorePlayerState->PlayerComponent->RoleID = RoleID;
        auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
        GameInstance->GameSystemManager->GetSystemByClass<UPlayerSystem>()->GamePlayers.Add(RoleID, this);
    }
}

void ACorePlayerController::ChangeCharacterTemplateId(int TemplateID) {
    if (GetLocalRole() != ROLE_Authority) {
        UE_LOG(GameCore, Error, TEXT("只能在服务器或者单机下更换角色模板"));
        return;
    }
    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
    if (UnitDataTable == nullptr) {
        UE_LOG(GameCore, Error, TEXT("没有配置单位表"));
        return;
    }
    auto FindTemplate = (FUnitInfoConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(UnitDataTable, TemplateID);
    if (FindTemplate == nullptr) {
        UE_LOG(GameCore, Error, TEXT("单位模板id找不到:%d"), TemplateID);
        return;
    }
    if (FindTemplate->CharacterClass.GetLongPackageName().IsEmpty()) {
        UE_LOG(GameCore, Error, TEXT("单位模板id角色类未配置:%d"), TemplateID);
        return;
    }
    auto CharacterClass = FindTemplate->CharacterClass.LoadSynchronous();
    if (!CharacterClass) {
        UE_LOG(GameCore, Error, TEXT("单位模板id角色类加载失败:%d"), TemplateID);
        return;
    }
    FTransform Transform = FTransform::Identity;
    auto OldCharacter = GetCharacter();
    if (OldCharacter) {
        auto OldTemplateID = Cast<ACoreCharacter>(OldCharacter)->TemplateID;
        if (OldTemplateID == TemplateID) {
            UE_LOG(GameCore, Error, TEXT("单位已经是该单位模板:%d"), TemplateID);
            return;
        }
        Transform = OldCharacter->GetTransform();
        OldCharacter->Destroy();
    }
    auto NewCharacter = Cast<ACoreCharacter>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, CharacterClass, Transform));
    if (!NewCharacter) {
        UE_LOG(GameCore, Error, TEXT("单位模板id角色类创建失败:%d"), TemplateID);
        return;
    }
    NewCharacter->TemplateID = TemplateID;
    NewCharacter->SetPlayerState(PlayerState);

    UGameplayStatics::FinishSpawningActor(NewCharacter, Transform);

	Possess(NewCharacter);

	OnCharacterTemplateUpdated.Broadcast(this);

    UE_LOG(GameCore, Log, TEXT("单位更换模板成功:%d"), TemplateID);
}

void ACorePlayerController::EnterGame(const FString& InName) {
	FString RequestURL = FString::Format(TEXT("{0}?RoleID={1}"), { InName, GetPlayerState<ACoreCharacterStateBase>()->PlayerComponent->RoleID });
    UE_LOG(GameCore, Log, TEXT("EnterGame:%s"), *RequestURL);
	ClientTravel(RequestURL, ETravelType::TRAVEL_Relative);
}

void ACorePlayerController::OnRep_Pawn() {
    Super::OnRep_Pawn();

    K2_OnClientPossessed();
}

void ACorePlayerController::SendEventToServerReliable_Implementation(const FString& EventClass, const FString& SerializeEvent) {
    auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->HandleSendEventToServer(this, EventClass, SerializeEvent);
}

void ACorePlayerController::SendEventToServerUnreliable_Implementation(const FString& EventClass, const FString& SerializeEvent) {
	auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->HandleSendEventToServer(this, EventClass, SerializeEvent);
}

void ACorePlayerController::SendEventToClient_Implementation(const FString& EventClass, const FString& SerializeEvent) {
    auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->HandleSendEventToClient(EventClass, SerializeEvent);
}

void ACorePlayerController::AddPawnInputContext(APawn* ToAdd)
{
	if (!ToAdd)
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			const TScriptInterface<IEnhancedInputSubsystemInterface> EnhancedInputInterface = PlayerSubsystem;
			if (auto CoreCharacter = Cast<ACoreCharacter>(ToAdd))
			{
				CoreCharacter->AddInputContext(EnhancedInputInterface);
			}
		}
	}
}

void ACorePlayerController::RemovePawnInputContext(APawn* ToRemove)
{
	if (!ToRemove)
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			const TScriptInterface<IEnhancedInputSubsystemInterface> EnhancedInputInterface = PlayerSubsystem;
			if (auto CoreCharacter = Cast<ACoreCharacter>(ToRemove))
			{
				CoreCharacter->RemoveInputContext(EnhancedInputInterface);
			}
		}
	}
}

bool ACorePlayerController::ProcessConsoleExec(const TCHAR * Cmd, FOutputDevice & Ar, UObject * Executor) {
    bool Res = Super::ProcessConsoleExec(Cmd, Ar, Executor);
    if (!Res) {
        if (GMComponent) {
            GMComponent->ProcessConsoleExec(Cmd, Ar, Executor);
        }
    }

    return Res;
}

void ACorePlayerController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
}

void ACorePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACorePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void ACorePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//蓝图属性默认值
	this->EnableInteractive = true;
}

void ACorePlayerController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void ACorePlayerController::SetPawn(APawn* InPawn) {
	APawn* PrevPawn = GetPawn();
	RemovePawnInputContext(PrevPawn);

	Super::SetPawn(InPawn);

	if (InPawn) {
		AddPawnInputContext(GetPawn());
	}
}