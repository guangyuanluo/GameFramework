// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreConditionProgress.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

void UCoreConditionProgress::OnInitialize_Implementation() {
    bInitialized = true;
}

void UCoreConditionProgress::OnUninitialize_Implementation() {
    bInitialized = false;
    bLastSatisfy = false;
}

bool UCoreConditionProgress::IsInitialized() const {
    return bInitialized;
}

bool UCoreConditionProgress::IsComplete_Implementation(bool& IsValid) {
    return false;
}

void UCoreConditionProgress::RefreshSatisfy() {
    bool IsValid;
    bool NewSatisfy = IsComplete(IsValid);
    if (!IsValid) {
        return;
    }
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

UWorld* UCoreConditionProgress::GetWorld() const {
    if (ProgressOwner) {
        return ProgressOwner->GetWorld();
    }
    return nullptr;
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