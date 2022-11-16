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

ACorePlayerController::ACorePlayerController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    GMComponent = CreateDefaultSubobject<UGMComponent>(TEXT("GMComponent"));
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

void ACorePlayerController::JumpExecute() {
	Jump();
}

void ACorePlayerController::StopJumpingExecute() {
	StopJumping();
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

void ACorePlayerController::MoveVertical(float Value)
{
	if (EnableInteractive && Value != 0.0f) {
		auto OwnPawn = GetPawn();
		if (OwnPawn) {
			const FRotator Rotation = GetControlRotation();
			FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation);
            OwnPawn->AddMovementInput(Direction, Value);
		}
	}
}

void ACorePlayerController::MoveHorizontal(float Value)
{
	if (EnableInteractive && Value != 0.0f) {
		auto OwnPawn = GetPawn();
		if (OwnPawn) {
			const FRotator Rotation = GetControlRotation();
			FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation);
            OwnPawn->AddMovementInput(Direction, Value);
		}
	}
}

void ACorePlayerController::Yaw(float Value)
{
	if (EnableInteractive && Value != 0.0f) {
		auto OwnPawn = GetPawn();
		if (OwnPawn) {
			// AddControllerYawInput()函数用于改变控制器的Yaw变量，即增加纵向轴旋转量。
			// GetWorld()函数取得世界指针UWorld*，通过世界指针调用GetDeltaSeconds()取得每帧耗费的时间。
			// 之所以要乘以每帧耗费的时间，是为了使得每一【秒】都增加200.0f * Value的改变量。
			// 如果不乘以每帧耗费的时间，那么每一【帧】都会增加200.0f * Value的改变量。（注意由于每秒渲染量不同，所以每秒的帧数不一定是固定的。）
			// 通过帧数来控制变量，那么游戏看起来就不那么流畅。试想，机子性能好的时候游戏角色动作就迅速，机子性能差的时候游戏角色动作就慢，这对于玩家公平吗？
			float Delta = 200.f * Value * GetWorld()->GetDeltaSeconds();
			if (Delta > MouseSensibility) Delta = MouseSensibility;
			else if (Delta < -MouseSensibility) Delta = -MouseSensibility;

            OwnPawn->AddControllerYawInput(Delta);
		}
	}
}

void ACorePlayerController::Pitch(float Value)
{
	if (EnableInteractive && Value != 0.0f) {
		auto OwnPawn = GetPawn();
		if (OwnPawn) {
			float Delta = 200.f * Value * GetWorld()->GetDeltaSeconds();
			if (Delta > MouseSensibility) Delta = MouseSensibility;
			else if (Delta < -MouseSensibility) Delta = -MouseSensibility;
            OwnPawn->AddControllerPitchInput(Delta);
		}
	}
}

void ACorePlayerController::Jump()
{
	if (EnableInteractive)
	{
		ACoreCharacter* CoreCharacter = Cast<ACoreCharacter>(GetPawn());
		CoreCharacter->Jump();
	}
}

void ACorePlayerController::StopJumping()
{
	if (EnableInteractive) {
		ACoreCharacter* CoreCharacter = Cast<ACoreCharacter>(GetPawn());
        CoreCharacter->StopJumping();
	}
}

void ACorePlayerController::OnTouchBegin(ETouchIndex::Type FingerIndex, FVector Location) {
	LastTouchLocation = FVector2D(Location);
}

void ACorePlayerController::OnTouchEnd(ETouchIndex::Type FingerIndex, FVector Location) {
	LastTouchLocation = FVector2D::ZeroVector;
}

static const float TouchRotationScale = 0.1f;

void ACorePlayerController::OnFingerMove(ETouchIndex::Type FingerIndex, FVector Location) {
	if (!LastTouchLocation.IsZero())
	{
		FVector2D const Delta = (FVector2D(Location) - LastTouchLocation) * TouchRotationScale;

		Yaw(Delta.X);
		Pitch(Delta.Y);

		LastTouchLocation = FVector2D(Location);
	}
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

	//绑定按键信息中指定字段到函数调用
	InputComponent->BindAxis("MoveVertical", this,
		&ACorePlayerController::MoveVertical);

	InputComponent->BindAxis("MoveHorizontal", this,
		&ACorePlayerController::MoveHorizontal);

	InputComponent->BindAxis("Yaw", this,
		&ACorePlayerController::Yaw);

	InputComponent->BindAxis("Pitch", this,
		&ACorePlayerController::Pitch);

	//绑定action
	InputComponent->BindAction("Jump", IE_Pressed, this,
		&ACorePlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this,
		&ACorePlayerController::StopJumping);

	InputComponent->BindTouch(IE_Pressed, this, &ACorePlayerController::OnTouchBegin);
	InputComponent->BindTouch(IE_Released, this, &ACorePlayerController::OnTouchEnd);
	InputComponent->BindTouch(IE_Repeat, this, &ACorePlayerController::OnFingerMove);

	//蓝图属性默认值
	this->EnableInteractive = true;
}

void ACorePlayerController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}