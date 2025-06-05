// Fill out your copyright notice in the Description page of Project Settings.


#include "Modules/Scenario/ScenarioComponent.h"
#include "Modules/Events/GameEventUtils.h"
#include "Base/ECS/GameEntity.h"
#include "GameFramework/PlayerState.h"
#include "Modules/Scenario/Scenario.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UScenarioComponent::UScenarioComponent()
: Super()
{
	SetIsReplicatedByDefault(true);
}

void UScenarioComponent::PlayScenario(TSoftObjectPtr<class UScenario> Scenario) {
    auto PlayerState = Cast<APlayerState>(GetOwner());
    check(PlayerState);

    auto GameEntity = Cast<IGameEntity>(PlayerState->GetPawn());
    if (GameEntity) {
        auto Request = NewObject<UPlayScenarioRequesEvent>();
        Request->EntityId = GameEntity->GetEntityID();
        FSoftObjectPath ScenarioObjectPath = Scenario.ToSoftObjectPath();
        Request->ScenarioPath = ScenarioObjectPath.ToString();

        UGameEventUtils::PushEventToServer(this, Request, false);
    }
}

void UScenarioComponent::Step(int ChildIndex) {
    auto PlayerState = Cast<APlayerState>(GetOwner());
    check(PlayerState);

    auto GameEntity = Cast<IGameEntity>(PlayerState->GetPawn());
    if (GameEntity) {
        auto Request = NewObject<UStepScenarioRequesEvent>();
        Request->EntityId = GameEntity->GetEntityID();
        Request->ChildIndex = ChildIndex;

        UGameEventUtils::PushEventToServer(this, Request, false);
    }
}

void UScenarioComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    FDoRepLifetimeParams Params;
    Params.Condition = COND_OwnerOnly;

    DOREPLIFETIME_WITH_PARAMS_FAST(UScenarioComponent, CurrentPlayScenarioNode, Params);
}

void UScenarioComponent::SetCurrent(class UScenarioNode* InScenarioNode) {
    auto World = GetWorld();
    auto NetMode = World->GetNetMode();
    if (NetMode == ENetMode::NM_DedicatedServer
        || NetMode == ENetMode::NM_Standalone) {
        if (InScenarioNode == CurrentPlayScenarioNode) {
            return;
        }

        CurrentPlayScenarioNode = InScenarioNode;
        OnScenarioUpdateDelegate.Broadcast(CurrentPlayScenarioNode);
    }
}

void UScenarioComponent::OnRep_CurrentPlayScenarioNode() {
    OnScenarioUpdateDelegate.Broadcast(CurrentPlayScenarioNode);
}