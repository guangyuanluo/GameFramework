// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

// Sets default values for this component's properties
UPlayerComponent::UPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPlayerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerComponent, RoleID);
	DOREPLIFETIME(UPlayerComponent, UnitID);
}
