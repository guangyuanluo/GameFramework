// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntityActor.h"
#include "Engine/World.h"
#include "CoreGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "EventSystem.h"
#include "GameEntityManager.h"
#include "AbilitySystemComponent.h"

AGameEntityActor::AGameEntityActor(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

FString& AGameEntityActor::EntityID_Implementation() {
	return EntityID;
}

void AGameEntityActor::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	if (EntityID.IsEmpty()) {
		EntityID = FGuid::NewGuid().ToString();
	}
}

const FTransform& AGameEntityActor::GetTransform() const {
    return GetActorTransform();
}

void AGameEntityActor::SetTransform(const FTransform& NewTransform) {
    AActor::SetActorTransform(NewTransform);
}

void AGameEntityActor::OnReq_EntityID(const FString& OldID) {
	auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
	GameInstance->GameEntityManager->OnEntityIDChanged(this, OldID);
}

void AGameEntityActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(AGameEntityActor, EntityID, COND_None, REPNOTIFY_Always);
}

void AGameEntityActor::BeginPlay()
{
	Super::BeginPlay();

	auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
	GameInstance->GameEntityManager->OnEntityAdd(this);
}

void AGameEntityActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	auto World = GetWorld();
	if (World) {
		auto GameInstance = World->GetGameInstance<UCoreGameInstance>();
		if (GameInstance && GameInstance->GameEntityManager) {
			GameInstance->GameEntityManager->OnEntityRemove(this);
		}
	}
}

UAbilitySystemComponent* AGameEntityActor::GetAbilitySystemComponent() const {
	return Cast<UAbilitySystemComponent>(GetComponentByClass(UAbilitySystemComponent::StaticClass()));
}