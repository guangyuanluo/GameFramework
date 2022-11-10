// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreReward.h"

UCoreReward::UCoreReward(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

FString UCoreReward::GetNodeTitle_Implementation() {
    return TEXT("");
}

void UCoreReward::HandleRewardDispatch_Implementation(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source) {

}