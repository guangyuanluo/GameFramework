// Fill out your copyright notice in the Description page of Project Settings.

#include "IntimacyReward.h"

UIntimacyReward::UIntimacyReward(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

FString UIntimacyReward::GetNodeTitle_Implementation() {
    return TEXT("好感度奖励");
}


void UIntimacyReward::HandleRewardDispatch_Implementation(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source) {

}