// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomizableSearchTreeEditor.h"

#include "GraphEditor.h"
#include "Modules/CustomizableSearchTree/CustomizableSearchTree.h"
#include "CustomizableSearchTreeGraph.h"
#include "CustomizableSearchTreeGraphSchema.h"
#include "Graph/GraphEditorModes.h"

const FName FCustomizableSearchTreeEditor::CustomizableSearchTreeMode(TEXT("CustomizableSearchTree"));
const FName CustomizableSearchTreeEditorAppIdentifier(TEXT("CustomizableSearchTreeEditorApp"));

FText FCustomizableSearchTreeEditor::GetLocalizedMode(FName InMode)
{
	static TMap< FName, FText > LocModes;

	if (LocModes.Num() == 0)
	{
		LocModes.Add(CustomizableSearchTreeMode, FText::FromString(TEXT("CustomizableSearchTree")));
	}

	check(InMode != NAME_None);
	const FText* OutDesc = LocModes.Find(InMode);
	check(OutDesc);
	return *OutDesc;
}

FName FCustomizableSearchTreeEditor::GetAppIdentifier() {
	return CustomizableSearchTreeEditorAppIdentifier;
}

TSharedRef<FApplicationMode> FCustomizableSearchTreeEditor::GetApplicationMode() {
	return MakeShareable(new FGraphEditorApplicationMode(SharedThis(this), FCustomizableSearchTreeEditor::CustomizableSearchTreeMode, FCustomizableSearchTreeEditor::GetLocalizedMode));
}

UGameFrameworkGraph* FCustomizableSearchTreeEditor::GetGraph() {
	auto CustomizableSearchTree = Cast<UCustomizableSearchTree>(GetObjectToEdit());
	return Cast<UGameFrameworkGraph>(CustomizableSearchTree->Graph);
}

void FCustomizableSearchTreeEditor::OnSetGraph(UGameFrameworkGraph* InGraph) {
	auto CustomizableSearchTree = Cast<UCustomizableSearchTree>(GetObjectToEdit());
	CustomizableSearchTree->Graph = InGraph;
}

TArray<FEditedDocumentInfo>& FCustomizableSearchTreeEditor::GetEditDocumentInfo() {
	auto CustomizableSearchTree = Cast<UCustomizableSearchTree>(GetObjectToEdit());
	return CustomizableSearchTree->LastEditedDocuments;
}

FName FCustomizableSearchTreeEditor::GetGraphName() const {
	return CustomizableSearchTreeMode;
}

TSubclassOf<class UEdGraph> FCustomizableSearchTreeEditor::GetGraphClass() {
	return UCustomizableSearchTreeGraph::StaticClass();
}

TSubclassOf<class UEdGraphSchema> FCustomizableSearchTreeEditor::GetSchemaClass() {
	return UCustomizableSearchTreeGraphSchema::StaticClass();
}