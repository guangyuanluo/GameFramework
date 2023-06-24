// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreConditionProgress.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

void UCoreConditionProgress::OnStart_Implementation() {

}

void UCoreConditionProgress::OnEnd_Implementation() {
    bLastSatisfy = false;
}

bool UCoreConditionProgress::IsComplete_Implementation() {
    return false;
}

void UCoreConditionProgress::RefreshSatisfy() {
    bool NewSatisfy = IsComplete();
    if (bLastSatisfy != NewSatisfy) {
        bLastSatisfy = NewSatisfy;
        OnSatisfyChange();
        OnConditionProgressSatisfyUpdate.Broadcast(this, bLastSatisfy);
    }
}

void UCoreConditionProgress::OnSatisfyChange_Implementation() {

}

void UCoreConditionProgress::GetProgressesWithChildren(TArray<UCoreConditionProgress*>& OutProgresses) {
    OutProgresses.Add(this);
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

void UCoreConditionProgress::PostNetReceive() {
    OnConditionProgressPostNetReceive.Broadcast(this);
}

void UCoreConditionProgress::OnRep_Condition() {
    
}