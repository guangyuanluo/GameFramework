// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreGameInstance.h"
#include "UE4LogImpl.h"

#include "Data/Team/TeamIDDefines.h"
#include "GenericTeamAgentInterface.h"
#include "GameEntityManager.h"
#include "GameSystemManager.h"
#include "GameInstanceSubSystemBase.h"

#include "ConfigTableCache.h"

ETeamAttitude::Type AttitudeSolver(FGenericTeamId TeamA, FGenericTeamId TeamB) {
	if (TeamA.GetId() == TeamB.GetId()) {
		return ETeamAttitude::Friendly;
	}
	if (TeamA.GetId() == (uint8)TeamIdDefines::E_Team16 || TeamB.GetId() == (uint8)TeamIdDefines::E_Team16) {
		return ETeamAttitude::Hostile;
	}
	if (TeamA.GetId() == (uint8)TeamIdDefines::E_Team15 || TeamB.GetId() == (uint8)TeamIdDefines::E_Team15) {
		return ETeamAttitude::Friendly;
	}
	return ETeamAttitude::Hostile;
}

UCoreGameInstance::UCoreGameInstance(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	FGenericTeamId::SetAttitudeSolver(AttitudeSolver);
}

void UCoreGameInstance::Init() {
	Super::Init();
	if (bAutoInitGameLogic) {
		InitGameLogic();
	}
}

void UCoreGameInstance::Shutdown() {
	Super::Shutdown();

	UConfigTableCache::Clear();

	GameEntityManager->RemoveFromRoot();
	GameSystemManager->Uninitialize();
	GameSystemManager->RemoveFromRoot();
}

void UCoreGameInstance::InitGameLogic() {
	if (!bInitialize) {
		bInitialize = true;

		//清除表数据
		UConfigTableCache::Clear();

		//初始化gameinstance子系统
		const auto& GameSubsystemArray = GetSubsystemArray<UGameInstanceSubsystemBase>();
		for (int Index = 0; Index < GameSubsystemArray.Num(); ++Index) {
			auto GameSubSystem = GameSubsystemArray[Index];
			GameSubSystem->Init();
		}

		//初始化实体管理
		GameEntityManager = NewObject<UGameEntityManager>(this, "GameEntityManager");
		GameEntityManager->AddToRoot();

		//初始化系统管理
		if (SystemManagerClass) {
			GameSystemManager = NewObject<UGameSystemManager>(this, SystemManagerClass, "GameSystemManager");
		}
		else {
			GameSystemManager = NewObject<UGameSystemManager>(this, "GameSystemManager");
		}
		GameSystemManager->AddToRoot();
		GameSystemManager->Initialize(this);
	}
}