// Fill out your copyright notice in the Description page of Project Settings.

#include "MoneyReward.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "AssetSystem.h"
#include "CoreCharacter.h"
#include "CoreCharacterStateBase.h"

UMoneyReward::UMoneyReward(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

FString UMoneyReward::GetNodeTitle_Implementation() {
    return TEXT("货币奖励");
}

void UMoneyReward::HandleRewardDispatch_Implementation(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source) {
    if (!Source) {
        return;
    }
    auto CharacterState = Cast<ACoreCharacterStateBase>(Source->GetPlayerState());
    if (!CharacterState) {
        return;
    }
    if (!CharacterState->WalletComponent) {
        return;
    }
    if (Money.Num() == 0) {
        return;
    }

    auto AssetSystem = InGameInstance->GameSystemManager->GetSystemByClass<UAssetSystem>();
    if (AssetSystem) {
        FString Error;
        AssetSystem->ChangeMoney(CharacterState->WalletComponent, Money, false, TEXT("Reward"), Error);
    }
}