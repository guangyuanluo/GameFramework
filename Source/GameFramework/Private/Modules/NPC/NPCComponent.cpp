// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCComponent.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "NPCSystem.h"
#include "GameFrameworkUtils.h"

// Sets default values for this component's properties
UNPCComponent::UNPCComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UNPCComponent::BeginPlay()
{
	Super::BeginPlay();

	auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
	if (GameInstance) {
		auto NPCSystem = GameInstance->GameSystemManager->GetSystemByClass<UNPCSystem>();
		if (NPCSystem) {
			auto Character = UGameFrameworkUtils::GetCharacterFromComponentOwner(this);
			if (Character) {
				NPCSystem->AddNPC(Character);
			}
		}
	}
}

void UNPCComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
	if (GameInstance) {
		auto NPCSystem = GameInstance->GameSystemManager->GetSystemByClass<UNPCSystem>();
		if (NPCSystem) {
			auto Character = UGameFrameworkUtils::GetCharacterFromComponentOwner(this);
			if (Character) {
				NPCSystem->RemoveNPC(Character);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}