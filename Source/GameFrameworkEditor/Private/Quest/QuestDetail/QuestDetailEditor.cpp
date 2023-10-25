// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestDetailEditor.h"
#include "Modules/Quest/QuestDetail.h"
#include "QuestDetailGraph.h"
#include "QuestDetailGraphSchema.h"
#include "Graph/GraphEditorModes.h"

const FName FQuestDetailEditor::QuestDetailMode(TEXT("QuestDetail"));
const FName QuestDetailEditorAppIdentifier(TEXT("QuestDetailEditorApp"));

FText FQuestDetailEditor::GetLocalizedMode(FName InMode)
{
	static TMap< FName, FText > LocModes;

	if (LocModes.Num() == 0)
	{
		LocModes.Add(QuestDetailMode, FText::FromString(TEXT("QuestDetail")));
	}

	check(InMode != NAME_None);
	const FText* OutDesc = LocModes.Find(InMode);
	check(OutDesc);
	return *OutDesc;
}

FName FQuestDetailEditor::GetAppIdentifier() {
	return QuestDetailEditorAppIdentifier;
}

TSharedRef<FApplicationMode> FQuestDetailEditor::GetApplicationMode() {
	return MakeShareable(new FGraphEditorApplicationMode(SharedThis(this), FQuestDetailEditor::QuestDetailMode, FQuestDetailEditor::GetLocalizedMode));
}

UGameFrameworkGraph* FQuestDetailEditor::GetGraph() {
	auto QuestDetail = Cast<UQuestDetail>(GetObjectToEdit());
	return Cast<UGameFrameworkGraph>(QuestDetail->QuestDetailGraph);
}

void FQuestDetailEditor::OnSetGraph(UGameFrameworkGraph* InGraph) {
	auto QuestDetail = Cast<UQuestDetail>(GetObjectToEdit());
	QuestDetail->QuestDetailGraph = InGraph;
}

TArray<FEditedDocumentInfo>& FQuestDetailEditor::GetEditDocumentInfo() {
	auto QuestDetail = Cast<UQuestDetail>(GetObjectToEdit());
	return QuestDetail->LastEditedDocuments;
}

FName FQuestDetailEditor::GetGraphName() const {
	return QuestDetailMode;
}

TSubclassOf<class UEdGraph> FQuestDetailEditor::GetGraphClass() {
	return UQuestDetailGraph::StaticClass();
}

TSubclassOf<class UEdGraphSchema> FQuestDetailEditor::GetSchemaClass() {
	return UQuestDetailGraphSchema::StaticClass();
}