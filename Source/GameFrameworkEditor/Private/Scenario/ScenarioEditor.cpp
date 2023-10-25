// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenarioEditor.h"

#include "GraphEditor.h"
#include "Modules/Scenario/Scenario.h"
#include "ScenarioGraph.h"
#include "ScenarioGraphSchema.h"
#include "Graph/GraphEditorModes.h"

const FName FScenarioEditor::ScenarioMode(TEXT("Scenario"));
const FName ScenarioEditorAppIdentifier(TEXT("ScenarioEditorApp"));

FText FScenarioEditor::GetLocalizedMode(FName InMode)
{
	static TMap< FName, FText > LocModes;

	if (LocModes.Num() == 0)
	{
		LocModes.Add(ScenarioMode, FText::FromString(TEXT("Scenario")));
	}

	check(InMode != NAME_None);
	const FText* OutDesc = LocModes.Find(InMode);
	check(OutDesc);
	return *OutDesc;
}

FName FScenarioEditor::GetAppIdentifier() {
	return ScenarioEditorAppIdentifier;
}

TSharedRef<FApplicationMode> FScenarioEditor::GetApplicationMode() {
	return MakeShareable(new FGraphEditorApplicationMode(SharedThis(this), FScenarioEditor::ScenarioMode, FScenarioEditor::GetLocalizedMode));
}

UGameFrameworkGraph* FScenarioEditor::GetGraph() {
	auto Scenario = Cast<UScenario>(GetObjectToEdit());
	return Cast<UGameFrameworkGraph>(Scenario->ScenarioGraph);
}

void FScenarioEditor::OnSetGraph(UGameFrameworkGraph* InGraph) {
	auto Scenario = Cast<UScenario>(GetObjectToEdit());
	Scenario->ScenarioGraph = InGraph;
}

TArray<FEditedDocumentInfo>& FScenarioEditor::GetEditDocumentInfo() {
	auto Scenario = Cast<UScenario>(GetObjectToEdit());
	return Scenario->LastEditedDocuments;
}

FName FScenarioEditor::GetGraphName() const {
	return ScenarioMode;
}

TSubclassOf<class UEdGraph> FScenarioEditor::GetGraphClass() {
	return UScenarioGraph::StaticClass();
}

TSubclassOf<class UEdGraphSchema> FScenarioEditor::GetSchemaClass() {
	return UScenarioGraphSchema::StaticClass();
}