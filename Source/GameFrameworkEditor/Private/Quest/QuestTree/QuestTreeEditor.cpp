// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestTreeEditor.h"
#include "Modules/Quest/QuestTree.h"
#include "QuestTreeGraph.h"
#include "QuestTreeGraphSchema.h"
#include "GraphEditorModes.h"

const FName FQuestTreeEditor::QuestTreeMode(TEXT("QuestTree"));
const FName QuestTreeEditorAppIdentifier(TEXT("QuestTreeEditorApp"));

FText FQuestTreeEditor::GetLocalizedMode(FName InMode)
{
	static TMap< FName, FText > LocModes;

	if (LocModes.Num() == 0)
	{
		LocModes.Add(QuestTreeMode, FText::FromString(TEXT("QuestTree")));
	}

	check(InMode != NAME_None);
	const FText* OutDesc = LocModes.Find(InMode);
	check(OutDesc);
	return *OutDesc;
}

FName FQuestTreeEditor::GetAppIdentifier() {
	return QuestTreeEditorAppIdentifier;
}

TSharedRef<FApplicationMode> FQuestTreeEditor::GetApplicationMode() {
	return MakeShareable(new FGraphEditorApplicationMode(SharedThis(this), FQuestTreeEditor::QuestTreeMode, FQuestTreeEditor::GetLocalizedMode));
}

UGameFrameworkGraph* FQuestTreeEditor::GetGraph() {
	auto QuestTree = Cast<UQuestTree>(GetObjectToEdit());
	if (!QuestTree->ID.IsValid()) {
		QuestTree->ID = FGuid::NewGuid();
		QuestTree->MarkPackageDirty();
	}
	return Cast<UGameFrameworkGraph>(QuestTree->QuestEdGraph);
}

void FQuestTreeEditor::OnSetGraph(UGameFrameworkGraph* InGraph) {
	auto QuestTree = Cast<UQuestTree>(GetObjectToEdit());
	QuestTree->QuestEdGraph = InGraph;
}

TArray<FEditedDocumentInfo>& FQuestTreeEditor::GetEditDocumentInfo() {
	auto QuestTree = Cast<UQuestTree>(GetObjectToEdit());
	return QuestTree->LastEditedDocuments;
}

FName FQuestTreeEditor::GetGraphName() const {
	return QuestTreeMode;
}

TSubclassOf<class UEdGraph> FQuestTreeEditor::GetGraphClass() {
	return UQuestTreeGraph::StaticClass();
}

TSubclassOf<class UEdGraphSchema> FQuestTreeEditor::GetSchemaClass() {
	return UQuestTreeGraphSchema::StaticClass();
}