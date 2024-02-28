// Fill out your copyright notice in the Description page of Project Settings.


#include "FindEnemyComponent.h"
#include "FindEnemyBase.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

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
	if (AutoUpdate) {
        if (!Enemy) {
			auto NetMode = GetWorld()->GetNetMode();
			if (NetMode != ENetMode::NM_Client && NetMode != ENetMode::NM_Standalone) {
				//索敌逻辑完全听客户端的
			}
			else {
				if (FindEnemyClass.Get()) {
					auto OldEnemy = Enemy;
					auto NewEnemy = FindEnemyObject->FindEnemy(this);
					if (NewEnemy != OldEnemy) {
						SetEnemy(NewEnemy);
						ServerSyncEnemy(NewEnemy);
					}
				}
			}
        }
	}
	return Enemy;
}

ACoreCharacter* UFindEnemyComponent::JustGetEnemy() const {
	return Enemy;
}

void UFindEnemyComponent::SetEnemy(ACoreCharacter* InEnemy) {
	auto OldEnemy = Enemy;
	Enemy = InEnemy;
	if (OldEnemy != Enemy) {
		OnTargetEnemyUpdate.Broadcast(this, OldEnemy, Enemy);
	}
}

void UFindEnemyComponent::ClearEnemy() {
	Enemy = nullptr;
}

void UFindEnemyComponent::SetAutoUpdateInterval(float NewInterval) {
	AutoUpdateInterval = NewInterval;
}

UFindEnemyBase* UFindEnemyComponent::GetFindEnemyInstance() const {
	return FindEnemyObject;
}

void UFindEnemyComponent::SetLock(bool bNewLock) {
	bLock = bNewLock;
}

bool UFindEnemyComponent::IsLock() const {
	return bLock;
}

void UFindEnemyComponent::BeginPlay() {
	Super::BeginPlay();

	if (!AutoUpdate) {
		SetComponentTickEnabled(false);
	}

	if (FindEnemyClass) {
		FindEnemyObject = NewObject<UFindEnemyBase>(this, FindEnemyClass);
	}
}

void UFindEnemyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	FindEnemyObject = nullptr;
}

void UFindEnemyComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto NetMode = GetWorld()->GetNetMode();
	if (LimitNetMode == 1) {
		//限制客户端
		if (NetMode != ENetMode::NM_Client && NetMode != ENetMode::NM_Standalone) {
			//索敌逻辑完全听客户端的
			return;
		}
	}
	else if (LimitNetMode == 2) {
		if (NetMode == ENetMode::NM_Client) {
			//索敌逻辑完全服务器的
			return;
		}
	}

	if (AutoUpdate) {
		FDateTime Now = FDateTime::Now();
		FTimespan TimeSinceLastUpdate = Now - LastAutoUpdateTime;
		if (TimeSinceLastUpdate.GetTotalMilliseconds() > AutoUpdateInterval) {
			LastAutoUpdateTime = Now;

			if (bLock) {
				return;
			}

			if (!IgnoreAutoUpdateTagContainer.IsEmpty()) {
				//有某些特定tag的时候，跳过自动更新
				if (auto ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner())) {
					if (ASC->HasAnyMatchingGameplayTags(IgnoreAutoUpdateTagContainer)) {
						return;
					}
				}
			}

			//更新索敌
			if (FindEnemyClass.Get()) {
				auto OldEnemy = Enemy;
				auto NewEnemy = FindEnemyObject->FindEnemy(this);
				if (NewEnemy != OldEnemy) {
					SetEnemy(NewEnemy);
					ServerSyncEnemy(NewEnemy);
				}
			}
		}
	}
}

void UFindEnemyComponent::ServerSyncEnemy_Implementation(ACoreCharacter* InEnemy) {
	SetEnemy(InEnemy);
}