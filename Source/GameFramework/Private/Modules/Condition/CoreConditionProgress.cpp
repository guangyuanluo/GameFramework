// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreConditionProgress.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

void UCoreConditionProgress::PostProgressInitialize_Implementation() {

}

void UCoreConditionProgress::StartFollow_Implementation() {

}

void UCoreConditionProgress::StopFollow_Implementation() {

}

bool UCoreConditionProgress::IsComplete_Implementation() {
    return false;
}

void UCoreConditionProgress::HandleComplete_Implementation() {

}

void UCoreConditionProgress::RefreshSatisfy() {
    bool NewSatisfy = IsComplete();
    if (LastSatisfy != NewSatisfy) {
        LastSatisfy = NewSatisfy;
        OnSatisfyChange();
        OnConditionProgressSatisfyUpdate.Broadcast(this, LastSatisfy);
    }
}

void UCoreConditionProgress::OnSatisfyChange_Implementation() {

}

void UCoreConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    FDoRepLifetimeParams Params;
    Params.Condition = COND_OwnerOnly;

    DOREPLIFETIME_WITH_PARAMS_FAST(UCoreConditionProgress, Condition, Params);
    DOREPLIFETIME_WITH_PARAMS_FAST(UCoreConditionProgress, ProgressOwner, Params);
}

bool UCoreConditionProgress::IsSupportedForNetworking() const {
    return true;
}

void UCoreConditionProgress::OnRep_Condition() {
    
}

void UCoreConditionProgress::PostNetReceive() {
    OnConditionProgressPostNetReceive.Broadcast(this);
}