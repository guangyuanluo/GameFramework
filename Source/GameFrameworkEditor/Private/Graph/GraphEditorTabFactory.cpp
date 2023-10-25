// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/GraphEditorTabFactory.h"

#include "Widgets/Docking/SDockTab.h"
#include "EditorStyleSet.h"
#include "Graph/GraphEditorBase.h"
#include "Engine/Blueprint.h"
#include "Graph/GameEditorTabs.h"

FGraphEditorSummoner::FGraphEditorSummoner(TSharedPtr<class FGraphEditorBase> InEditorPtr, FOnCreateGraphEditorWidget CreateGraphEditorWidgetCallback)
	: FDocumentTabFactoryForObjects<UEdGraph>(FGameEditorTabs::GraphEditorID, InEditorPtr)
	, EditorPtr(InEditorPtr)
	, OnCreateGraphEditorWidget(CreateGraphEditorWidgetCallback)
{
}

void FGraphEditorSummoner::OnTabActivated(TSharedPtr<SDockTab> Tab) const
{
	check(EditorPtr.IsValid());
	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	EditorPtr.Pin()->OnGraphEditorFocused(GraphEditor);
}

void FGraphEditorSummoner::OnTabRefreshed(TSharedPtr<SDockTab> Tab) const
{
	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	GraphEditor->NotifyGraphChanged();
}

TAttribute<FText> FGraphEditorSummoner::ConstructTabNameForObject(UEdGraph* DocumentID) const
{
	return TAttribute<FText>(FText::FromString(DocumentID->GetName()));
}

TSharedRef<SWidget> FGraphEditorSummoner::CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info, UEdGraph* DocumentID) const
{
	return OnCreateGraphEditorWidget.Execute(DocumentID);
}

const FSlateBrush* FGraphEditorSummoner::GetTabIconForObject(const FWorkflowTabSpawnInfo& Info, UEdGraph* DocumentID) const
{
#if ENGINE_MAJOR_VERSION > 4
	return FAppStyle::GetBrush("NoBrush");
#else
	return FEditorStyle::GetBrush("NoBrush");	
#endif
}

void FGraphEditorSummoner::SaveState(TSharedPtr<SDockTab> Tab, TSharedPtr<FTabPayload> Payload) const
{
	check(EditorPtr.IsValid());
	check(EditorPtr.Pin()->GetObjectToEdit());

	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());

	FVector2D ViewLocation;
	float ZoomAmount;
	GraphEditor->GetViewLocation(ViewLocation, ZoomAmount);

	UEdGraph* Graph = FTabPayload_UObject::CastChecked<UEdGraph>(Payload);
	EditorPtr.Pin()->GetEditDocumentInfo().Add(FEditedDocumentInfo(Graph, ViewLocation, ZoomAmount));
}

FGraphDetailsSummoner::FGraphDetailsSummoner(TSharedPtr<class FGraphEditorBase> InEditorPtr)
	: FWorkflowTabFactory(FGameEditorTabs::GraphDetailsID, InEditorPtr)
	, EditorPtr(InEditorPtr)
{
	TabLabel = FText::FromString(TEXT("Details"));
#if ENGINE_MAJOR_VERSION > 4
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "Kismet.Tabs.Components");
#else
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "Kismet.Tabs.Components");
#endif

	bIsSingleton = true;

	ViewMenuDescription = FText::FromString(TEXT("Details"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the details view"));
}

TSharedRef<SWidget> FGraphDetailsSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(EditorPtr.IsValid());
	return EditorPtr.Pin()->SpawnProperties();
}

FText FGraphDetailsSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("The graph details tab allows editing of the properties of behavior tree nodes"));
}