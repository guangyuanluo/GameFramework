// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreConditionProgress.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "CoreCharacterStateBase.h"
#include "CoreAbilitySystemComponent.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "ConditionGlobalSystem.h"

void UCoreConditionProgress::Initialize() {
    OnInitialize();

    bInitialized = true;
}

void UCoreConditionProgress::Uninitialize() {
    OnUninitialize();

    auto ConditionGlobalSystem = Cast<UConditionGlobalSystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(ProgressOwner, UConditionGlobalSystem::StaticClass()));
    ConditionGlobalSystem->FreeProgress(this);

    bInitialized = false;
    bLastSatisfy = false;
}

bool UCoreConditionProgress::IsInitialized() const {
    return bInitialized;
}

bool UCoreConditionProgress::IsComplete(bool& IsValid) {
    return K2_IsComplete(IsValid);
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

void UCoreConditionProgress::OnSatisfyChange() {
    K2_OnSatisfyChange();
}

void UCoreConditionProgress::GetProgressesWithChildren(TArray<UCoreConditionProgress*>& OutProgresses) {
    OutProgresses.Add(this);
}

class UCoreAbilitySystemComponent* UCoreConditionProgress::GetAbilitySystemComponent() const {
    return Cast<UCoreAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ProgressOwner));
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

void UCoreConditionProgress::OnInitialize() {
    K2_OnInitialize();
}

void UCoreConditionProgress::OnUninitialize() {
    K2_OnUninitialize();
}