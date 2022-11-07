// Fill out your copyright notice in the Description page of Project Settings.


#include "WalletComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

#include "CoreGameInstance.h"

#include "AssetSystem.h"
#include "GameSystemManager.h"
#include "EventSystem.h"
#include "GameEntity.h"
#include "PlayerState/CoreCharacterStateBase.h"

// Sets default values for this component's properties
UWalletComponent::UWalletComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UWalletComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UWalletComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UWalletComponent, Wallets);
}

void UWalletComponent::ChangeMoney(uint8 MoneyType, int32 Count, bool bConsume, const FString& Reason) {
    auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();

    auto PlayerState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (PlayerState) {
        auto GameEntity = Cast<IGameEntity>(PlayerState->GetPawn());
        if (GameEntity) {
            auto Request = NewObject<UChangeMoneyRequestEvent>();
            Request->EntityId = GameEntity->GetEntityID();
            Request->MoneyType = MoneyType;
            Request->Count = Count;
            Request->bConsume = bConsume;
            Request->Reason = Reason;

            GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEventToServer(Request, false);
        }
    }
}

int UWalletComponent::GetMoneyCount(uint8 MoneyType) {
    for (int Index = 0; Index < Wallets.Num(); ++Index) {
        if (Wallets[Index].MoneyType == MoneyType) {
            return Wallets[Index].Count;
        }
    }
    return 0;
}

void UWalletComponent::OnWalletRefresh() {
    auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();

    auto PlayerState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (PlayerState) {
        auto GameEntity = Cast<IGameEntity>(PlayerState->GetPawn());
        if (GameEntity) {
            auto OnWalletRefreshEvent = NewObject<UOnWalletRefreshEvent>();
            OnWalletRefreshEvent->WalletComponent = this;

            GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(OnWalletRefreshEvent);
        }
    }
}