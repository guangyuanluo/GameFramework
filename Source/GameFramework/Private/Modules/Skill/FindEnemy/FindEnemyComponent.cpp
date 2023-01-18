// Fill out your copyright notice in the Description page of Project Settings.


#include "FindEnemyComponent.h"
#include "FindEnemyBase.h"

// Sets default values for this component's properties
UFindEnemyComponent::UFindEnemyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	SetIsReplicatedByDefault(true);
	// ...
}

ACoreCharacter* UFindEnemyComponent::FindOrGetEnemy() {
	if (!Enemy) {
		if (FindEnemyClass.Get()) {
			Enemy = FindEnemyObject->FindEnemy(this);
		}
	}
	return Enemy;
}

ACoreCharacter* UFindEnemyComponent::JustGetEnemy() const {
	return Enemy;
}

void UFindEnemyComponent::SetEnemy(ACoreCharacter* InEnemy) {
	Enemy = InEnemy;
}

void UFindEnemyComponent::ClearEnemy() {
	Enemy = nullptr;
}

void UFindEnemyComponent::BeginPlay() {
	Super::BeginPlay();

	FindEnemyObject = NewObject<UFindEnemyBase>(this, FindEnemyClass);
}

void UFindEnemyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	FindEnemyObject = nullptr;
}

void UFindEnemyComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindEnemyObject->Tick(this);
}