// Fill out your copyright notice in the Description page of Project Settings.

#include "IntimacyReward.h"

UIntimacyReward::UIntimacyReward(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	RewardType = RewardType::E_Intimacy;
}

void UIntimacyReward::HandleRewardDispatch(UCoreGameInstance* gameInstance, TScriptInterface<class IGameEntity> target) {

}