// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

#include "GameEventUtils.h"
#include "GameEntity.h"
#include "Engine/World.h"
#include "PlayerState/CoreCharacterStateBase.h"
#include "GameFrameworkUtils.h"

#include "UE4LogImpl.h"

// Sets default values for this component's properties
UExpComponent::UExpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UExpComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UExpComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UExpComponent, Exps);
}

int UExpComponent::GetExpLevel(EExpTypeEnum ExpType) const {
	for (int Index = 0; Index < Exps.Num(); ++Index) {
		if (Exps[Index].ExpType == ExpType) {
			return Exps[Index].Level;
		}
	}
	return 1;
}

int UExpComponent::GetExpValue(EExpTypeEnum ExpType) const {
	for (int Index = 0; Index < Exps.Num(); ++Index) {
		if (Exps[Index].ExpType == ExpType) {
			return Exps[Index].Exp;
		}
	}
	UE_LOG(GameCore, Warning, TEXT("经验组件没有对应类型的经验:%d"), ExpType);
	return 0;
}

void UExpComponent::AddExp(EExpTypeEnum ExpType, int32 Exp, const FString& Reason) {
    auto PlayerState = Cast<ACoreCharacterStateBase>(GetOwner());
    if (PlayerState) {
        auto GameEntity = Cast<IGameEntity>(PlayerState->GetPawn());
        if (GameEntity) {
            auto AddExpRequest = NewObject<UAddExpRequesEvent>();
            AddExpRequest->EntityId = GameEntity->GetEntityID();
            AddExpRequest->ExpType = ExpType;
            AddExpRequest->Exp = Exp;
            AddExpRequest->Reason = Reason;

            UGameEventUtils::PushEventToServer(this, AddExpRequest, false);
        }
	}
}

void UExpComponent::OnExpChanged() {
    auto OnExpRefreshEvent = NewObject<UOnExpRefreshEvent>();
    OnExpRefreshEvent->Character = UGameFrameworkUtils::GetCharacterFromComponentOwner(this);
	UGameEventUtils::PushEvent(this, OnExpRefreshEvent);
}