// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemReward.h"
#include "GameFrameworkUtils.h"
#include "CoreGameInstance.h"
#include "BackpackComponent.h"
#include "GameSystemManager.h"
#include "AssetSystem.h"
#include "CoreCharacterStateBase.h"

UItemReward::UItemReward(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	RewardType = RewardType::E_Item;
}

void UItemReward::HandleRewardDispatch(UCoreGameInstance* GameInstance, TScriptInterface<class IGameEntity> Target) {
	auto CharacterState = UGameFrameworkUtils::GetEntityState(Target);
	if (CharacterState && CharacterState->BackpackComponent) {
		FString Error;
		GameInstance->GameSystemManager->GetSystemByClass<UAssetSystem>()->AddItem(CharacterState->BackpackComponent, BackpackType, ItemId, Count, -1, true, TEXT("Reward"), Error);
	}
}