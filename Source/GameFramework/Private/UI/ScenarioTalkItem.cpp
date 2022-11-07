// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenarioTalkItem.h"
#include "CoreGameInstance.h"

UScenarioNode* UScenarioTalkItem::GetScenarioRootNode() {
	UScenario* LoadScenario = Scenario.LoadSynchronous();
	return LoadScenario->RootScenario;
}