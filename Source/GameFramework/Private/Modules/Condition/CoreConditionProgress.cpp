// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreConditionProgress.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "ConditionStoreSubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

void UCoreConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UCoreConditionProgress::GetCareEventTypes_Implementation() {
    return {};
}

bool UCoreConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
    return false;
}

bool UCoreConditionProgress::IsComplete_Implementation() {
    return false;
}

void UCoreConditionProgress::HandleComplete_Implementation() {

}

void UCoreConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCoreConditionProgress, ConditionID);
    DOREPLIFETIME(UCoreConditionProgress, ProgressOwner);
}

bool UCoreConditionProgress::IsSupportedForNetworking() const {
    return true;
}

void UCoreConditionProgress::OnRep_ConditionID() {
    UConditionStoreSubsystem* ConditionStoreSubsystem = Cast<UConditionStoreSubsystem>(USubsystemBlueprintLibrary::GetGameInstanceSubsystem(this, UConditionStoreSubsystem::StaticClass()));
    Condition = ConditionStoreSubsystem->GetCondition(ConditionID);
}