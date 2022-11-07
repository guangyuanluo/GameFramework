// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenarioFactory.h"

#include "Modules/Scenario/Scenario.h"

UScenarioFactory::UScenarioFactory(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SupportedClass = UScenario::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UScenarioFactory::CanCreateNew() const
{
	return true;
}

UObject* UScenarioFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UScenario::StaticClass()));
	return NewObject<UScenario>(InParent, Class, Name, Flags);;
}