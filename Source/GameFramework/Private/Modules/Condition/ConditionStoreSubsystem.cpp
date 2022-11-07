// Fill out your copyright notice in the Description page of Project Settings.

#include "ConditionStoreSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UConditionStoreSubsystem::RegistCondition(UCoreCondition* InCondition) {
    if (InCondition->ID.IsValid()) {
        ConditionMap.Add(InCondition->ID, InCondition);
    }
}

void UConditionStoreSubsystem::UnregistCondition(UCoreCondition* InCondition) {
    if (InCondition->ID.IsValid()) {
        ConditionMap.Remove(InCondition->ID);
    }
}

UCoreCondition* UConditionStoreSubsystem::GetCondition(const FGuid& InGuid) {
    auto FindConditionPtr = ConditionMap.Find(InGuid);
    if (FindConditionPtr) {
        return *FindConditionPtr;
    }
    else {
        return nullptr;
    }
}