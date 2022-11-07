// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenarioNodeReturn.h"

UScenarioNodeReturn::UScenarioNodeReturn(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

FString UScenarioNodeReturn::GetNodeTitle_Implementation() {
    if (IsTemplate()) {
        return TEXT("返回分支");
    }
    else {
        return FString::Format(TEXT("返回:{0}"), { ReturnBranch });
    }
}

TArray<FScenarioNodeOutputPinInfo> UScenarioNodeReturn::GetOutputPinInfo_Implementation() {
    return TArray<FScenarioNodeOutputPinInfo>();
}