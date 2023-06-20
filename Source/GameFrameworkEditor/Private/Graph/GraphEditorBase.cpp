// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphEditorBase.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/GameFrameworkGraphNode.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "GameFrameworkGraph.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"
#include "GraphEditorModes.h"
#include "GraphEditorTabFactory.h"
#include "EdGraphUtilities.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Widgets/Layout/SScrollBox.h"

FGraphEditorBase::FGraphEditorBase() {

}

FGraphEditorBase::~FGraphEditorBase() {

}

void FGraphEditorBase::RegisterTabSpawnersBase(const TSharedRef<class FTabManager>& InTabManager) {
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FGraphEditorBase::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) {
	DocumentManager->SetTabManager(InTabManager);

	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FGraphEditorBase::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UObject* InObject) {
	ObjectToEdit = InObject;

	TSharedPtr<FGraphEditorBase> ThisPtr(SharedThis(this));
	if (!DocumentManager.IsValid()) {
		DocumentManager = MakeShareable(new FDocumentTracker);
		DocumentManager->Initialize(ThisPtr);

		// Register the document factories
		{
			TSharedRef<FDocumentTabFactory> GraphEditorFactory = GetDocumentFactory();

			// Also store off a reference to the grapheditor Factory so we can find all the tabs spawned by it later.
			GraphEditorTabFactoryPtr = GraphEditorFactory;
			DocumentManager->RegisterDocumentFactory(GraphEditorFactory);
		}
	}

	TArray<UObject*> ObjectsToEdit;
	if (ObjectToEdit != nullptr) {
		ObjectsToEdit.Add(ObjectToEdit);
	}
	// if we are already editing objects, dont try to recreate the editor from scratch
	const TArray<UObject*>* EditedObjects = GetObjectsCurrentlyBeingEdited();
	if (EditedObjects == nullptr || EditedObjects->Num() == 0) {
		FGraphEditorCommands::Register();

		const bool bCreateDefaultStandaloneMenu = true;
		const bool bCreateDefaultToolbar = true;
		InitAssetEditor(Mode, InitToolkitHost, GetAppIdentifier(), FTabManager::FLayout::NullLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectsToEdit);

		AddApplicationMode(GetGraphName(), GetApplicationMode());

		CreateInternalWidgets();
	}

	if (ObjectToEdit != nullptr) {
		SetCurrentMode(GetGraphName());
	}
}

void FGraphEditorBase::SaveEditedObjectState() {
	// Clear currently edited documents
	GetEditDocumentInfo().Empty();

	// Ask all open documents to save their state, which will update LastEditedDocuments
	DocumentManager->SaveAllState();
}

void FGraphEditorBase::RestoreObject() {
	// Update asset data based on saved graph to have correct data in editor
	UGameFrameworkGraph* MyGraph = GetGraph();
	const bool bNewGraph = MyGraph == NULL;
	if (MyGraph == NULL) {
		MyGraph = Cast<UGameFrameworkGraph>(FBlueprintEditorUtils::CreateNewGraph(ObjectToEdit, GetGraphName(), GetGraphClass(), GetSchemaClass()));
		MyGraph->Editor = this;
		OnSetGraph(MyGraph);

		// Initialize the behavior tree graph
		const UEdGraphSchema* Schema = MyGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*MyGraph);
	}
	else {
		MyGraph->Editor = this;
	}

	TSharedRef<FTabPayload_UObject> Payload = FTabPayload_UObject::Make(MyGraph);
	TSharedPtr<SDockTab> DocumentTab = DocumentManager->OpenDocument(Payload, bNewGraph ? FDocumentTracker::OpenNewDocument : FDocumentTracker::RestorePreviousDocument);

	if (GetEditDocumentInfo().Num() > 0) {
		TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(DocumentTab->GetContent());
		GraphEditor->SetViewLocation(GetEditDocumentInfo()[0].SavedViewOffset, GetEditDocumentInfo()[0].SavedZoomAmount);
	}
}

void FGraphEditorBase::PostUndo(bool bSuccess) {
	if (bSuccess) {
		// Clear selection, to avoid holding refs to nodes that go away
		if (TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin()) {
			CurrentGraphEditor->ClearSelectionSet();
			CurrentGraphEditor->NotifyGraphChanged();
		}
		FSlateApplication::Get().DismissAllMenus();
	}
}

void FGraphEditorBase::PostRedo(bool bSuccess) {
	if (bSuccess) {
		// Clear selection, to avoid holding refs to nodes that go away
		if (TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin()) {
			CurrentGraphEditor->ClearSelectionSet();
			CurrentGraphEditor->NotifyGraphChanged();
		}
		FSlateApplication::Get().DismissAllMenus();
	}
}

void FGraphEditorBase::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) {
	UGameFrameworkGraph* Graph = GetGraph();;
	if (Graph) {
		Graph->RefreshAllNodesOnDataChanged();
	}
}

TSharedRef<SWidget> FGraphEditorBase::SpawnProperties() {
	TSharedPtr<SScrollBar> VerticalScrollBar = SNew(SScrollBar)
		.AlwaysShowScrollbar(true);
	return
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			SNew(SScrollBox)
			.Orientation(Orient_Vertical)
			.ExternalScrollbar(VerticalScrollBar)
			+ SScrollBox::Slot()
			[
				DetailsView.ToSharedRef()
			]
		];
}

void FGraphEditorBase::OnGraphEditorFocused(const TSharedRef<SGraphEditor>& InGraphEditor) {
	UpdateGraphEdPtr = InGraphEditor;

	FGraphPanelSelectionSet CurrentSelection;
	CurrentSelection = InGraphEditor->GetSelectedNodes();
	OnSelectedNodesChanged(CurrentSelection);
}

TSharedRef<class SGraphEditor> FGraphEditorBase::CreateGraphEditorWidget(UEdGraph* InGraph)
{
	check(InGraph != NULL);

	if (!GraphEditorCommands.IsValid())
	{
		CreateCommandList();
	}

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FGraphEditorBase::OnSelectedNodesChanged);

	// Make title bar
	TSharedRef<SWidget> TitleBarWidget =
		SNew(SBorder)
#if ENGINE_MAJOR_VERSION > 4
		.BorderImage(FAppStyle::GetBrush(TEXT("Graph.TitleBackground")))
#else
		.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
#endif
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(1.f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("图表")))
#if ENGINE_MAJOR_VERSION > 4
				.TextStyle(FAppStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
#else
				.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
#endif
			]
		];

	// Make full graph editor
	const bool bGraphIsEditable = InGraph->bEditable;
	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(this, &FGraphEditorBase::InEditingMode, bGraphIsEditable)
		.TitleBar(TitleBarWidget)
		.GraphToEdit(InGraph)
		.GraphEvents(InEvents);
}

void FGraphEditorBase::CreateCommandList() {
	if (GraphEditorCommands.IsValid()) {
		return;
	}

	GraphEditorCommands = MakeShareable(new FUICommandList);

	GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FGraphEditorBase::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FGraphEditorBase::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FGraphEditorBase::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FGraphEditorBase::CanCutNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FGraphEditorBase::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FGraphEditorBase::CanCopyNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FGraphEditorBase::PasteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FGraphEditorBase::CanPasteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FGraphEditorBase::DuplicateSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FGraphEditorBase::CanDuplicateNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FGraphEditorBase::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FGraphEditorBase::CanDeleteNodes)
	);
}

void FGraphEditorBase::SelectAllNodes() {
	if (TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin())
		CurrentGraphEditor->SelectAllNodes();
}

bool FGraphEditorBase::CanSelectAllNodes() const {
	return true;
}

void FGraphEditorBase::CutSelectedNodes() {
	if (TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin()) {
		CopySelectedNodes();

		const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
		CurrentGraphEditor->ClearSelectionSet();
		for (FGraphPanelSelectionSet::TConstIterator It(OldSelectedNodes); It; ++It) {
			UEdGraphNode* Node = Cast<UEdGraphNode>(*It);
			if (Node && Node->CanDuplicateNode()) {
				CurrentGraphEditor->SetNodeSelection(Node, true);
			}
		}

		DeleteSelectedNodes();

		CurrentGraphEditor->ClearSelectionSet();

		for (FGraphPanelSelectionSet::TConstIterator It(OldSelectedNodes); It; ++It) {
			UEdGraphNode* Node = Cast<UEdGraphNode>(*It);
			if (Node)
				CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

bool FGraphEditorBase::CanCutNodes() const {
	return true;
}

void FGraphEditorBase::CopySelectedNodes() {
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	FString ExportedText;

	for (FGraphPanelSelectionSet::TIterator it(SelectedNodes); it; ++it) {
		UEdGraphNode* Node = Cast<UEdGraphNode>(*it);
		if (Node)
			Node->PrepareForCopying();
		else
			it.RemoveCurrent();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);

	for (FGraphPanelSelectionSet::TIterator it(SelectedNodes); it; ++it) {
		UGameFrameworkGraphNode* Node = Cast<UGameFrameworkGraphNode>(*it);
		if (Node)
			Node->PostCopyNode();
		else
			it.RemoveCurrent();
	}
}

bool FGraphEditorBase::CanCopyNodes() const {
	return true;
}

void FGraphEditorBase::PasteSelectedNodes() {
	if (TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin()) {
		const FVector2D PasteLocation = CurrentGraphEditor->GetPasteLocation();

		UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();
		EdGraph->Modify();
		CurrentGraphEditor->ClearSelectionSet();

		FString ExportedText;
		FPlatformApplicationMisc::ClipboardPaste(ExportedText);
		TSet<UEdGraphNode*> ImportedNodes;
		FEdGraphUtilities::ImportNodesFromText(EdGraph, ExportedText, ImportedNodes);

		FVector2D AvgNodePosition(0.0f, 0.0f);

		for (TSet<UEdGraphNode*>::TIterator It(ImportedNodes); It; ++It) {
			UEdGraphNode* Node = *It;
			AvgNodePosition.X += Node->NodePosX;
			AvgNodePosition.Y += Node->NodePosY;
		}

		float InvNumNodes = 1.0f / float(ImportedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;

		for (TSet<UEdGraphNode*>::TIterator It(ImportedNodes); It; ++It) {
			UEdGraphNode* Node = *It;
			CurrentGraphEditor->SetNodeSelection(Node, true);

			Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + PasteLocation.X;
			Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + PasteLocation.Y;

			Node->SnapToGrid(16);

			// Give new node a different Guid from the old one
			Node->CreateNewGuid();
		}

		CurrentGraphEditor->NotifyGraphChanged();

		UObject* GraphOwner = EdGraph->GetOuter();
		if (GraphOwner) {
			GraphOwner->PostEditChange();
			GraphOwner->MarkPackageDirty();
		}
	}
}

bool FGraphEditorBase::CanPasteNodes() const {
	return true;
}

void FGraphEditorBase::DuplicateSelectedNodes() {
	CopySelectedNodes();
	PasteSelectedNodes();
}

bool FGraphEditorBase::CanDuplicateNodes() const {
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter) {
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode()) {
			return true;
		}
	}

	return false;
}

void FGraphEditorBase::DeleteSelectedNodes() {
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (!CurrentGraphEditor.IsValid()) {
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt) {
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt)) {
			if (Node->CanUserDeleteNode()) {
				Node->Modify();
				Node->DestroyNode();
			}
		}
	}
}

bool FGraphEditorBase::CanDeleteNodes() const {
	// If any of the nodes can be deleted then we should allow deleting
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter) {
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanUserDeleteNode()) {
			return true;
		}
	}

	return false;
}

void FGraphEditorBase::CreateInternalWidgets() {
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NotifyHook = this;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(ObjectToEdit);
	DetailsView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &FGraphEditorBase::IsPropertyEditable));	
	DetailsView->OnFinishedChangingProperties().AddSP(this, &FGraphEditorBase::OnFinishedChangingProperties);
}

FGraphPanelSelectionSet FGraphEditorBase::GetSelectedNodes() const {
	FGraphPanelSelectionSet CurrentSelection;
	if (TSharedPtr<SGraphEditor> FocusedGraphEd = UpdateGraphEdPtr.Pin()) {
		CurrentSelection = FocusedGraphEd->GetSelectedNodes();
	}

	return CurrentSelection;
}

bool FGraphEditorBase::IsPropertyEditable() const {
	return true;
}

void FGraphEditorBase::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent) {
	auto Graph = GetGraph();
	if (Graph) {
		Graph->GetSchema()->ForceVisualizationCacheClear();
	}
}

bool FGraphEditorBase::InEditingMode(bool bGraphIsEditable) const {
	return bGraphIsEditable;
}

void FGraphEditorBase::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection) {
	if (NewSelection.Num() == 1) {
		for (UObject* SelectionEntry : NewSelection) {
			UGameFrameworkGraphNode* GraphNode = Cast<UGameFrameworkGraphNode>(SelectionEntry);
			if (GraphNode != nullptr) {
				DetailsView->SetObject(GraphNode->NodeInstance);
			}
		}
	}
	else {
		DetailsView->SetObject(nullptr);
	}
}

FName FGraphEditorBase::GetToolkitFName() const {
	return GetGraphName();
}

FText FGraphEditorBase::GetBaseToolkitName() const {
	return FText::FromString(GetGraphName().ToString());
}

FString FGraphEditorBase::GetWorldCentricTabPrefix() const {
	return GetGraphName().ToString();
}

FLinearColor FGraphEditorBase::GetWorldCentricTabColorScale() const {
	return FLinearColor(0.0f, 0.0f, 0.2f, 0.5f);
}

FText FGraphEditorBase::GetToolkitName() const {
	const UObject* EditingObject = GetCurrentMode() == GetGraphName() ? ObjectToEdit : nullptr;
	if (EditingObject != nullptr) {
		return FAssetEditorToolkit::GetLabelForObject(EditingObject);
	}

	return FText();
}

FText FGraphEditorBase::GetToolkitToolTipText() const {
	const UObject* EditingObject = GetCurrentMode() == GetGraphName() ? ObjectToEdit : nullptr;
	if (EditingObject != nullptr) {
		return FAssetEditorToolkit::GetToolTipTextForObject(EditingObject);
	}

	return FText();
}

TSharedRef<FDocumentTabFactory> FGraphEditorBase::GetDocumentFactory() {
	return MakeShareable(new FGraphEditorSummoner(SharedThis(this),
		FGraphEditorSummoner::FOnCreateGraphEditorWidget::CreateSP(this, &FGraphEditorBase::CreateGraphEditorWidget)
	));
}