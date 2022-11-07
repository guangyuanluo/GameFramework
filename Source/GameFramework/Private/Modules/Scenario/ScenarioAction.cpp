// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenarioAction.h"

UScenarioAction::UScenarioAction(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

void UScenarioAction::Execute_Implementation(class UScenario* Scenario, class UScenarioNode* ScenarioNode) {

}

FString UScenarioAction::GetNodeTitle_Implementation() {
    return TEXT("");
}