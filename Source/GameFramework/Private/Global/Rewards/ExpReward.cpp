// Fill out your copyright notice in the Description page of Project Settings.

#include "ExpReward.h"

UExpReward::UExpReward(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

FString UExpReward::GetNodeTitle_Implementation() {
    return TEXT("经验奖励");
}

void UExpReward::HandleRewardDispatch_Implementation(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source) {

}