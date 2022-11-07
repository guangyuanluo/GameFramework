// Fill out your copyright notice in the Description page of Project Settings.

#include "MoneyReward.h"
#include "GameFrameworkUtils.h"
#include "CoreGameInstance.h"
#include "WalletComponent.h"
#include "GameSystemManager.h"
#include "AssetSystem.h"
#include "CoreCharacterStateBase.h"

UMoneyReward::UMoneyReward(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	RewardType = RewardType::E_Money;
}

void UMoneyReward::HandleRewardDispatch(UCoreGameInstance* GameInstance, TScriptInterface<class IGameEntity> Target) {
	auto PlayerState = UGameFrameworkUtils::GetEntityState(Target);
	if (PlayerState && PlayerState->WalletComponent) {
		FString Error;
		GameInstance->GameSystemManager->GetSystemByClass<UAssetSystem>()->ChangeMoney(PlayerState->WalletComponent, MoneyType, MoneyCount, false, TEXT("Reward"), Error);
	}
}