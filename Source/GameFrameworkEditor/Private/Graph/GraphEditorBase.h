// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EditorUndoClient.h"
#include "Misc/NotifyHook.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class FDocumentTracker;
class SGraphEditor;
class FDocumentTabFactory;
class UEdGraph;
class IDetailsView;
class UGameFrameworkGraph;
/**
 * 
 */
class FGraphEditorBase : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	FGraphEditorBase();
	virtual ~FGraphEditorBase();

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	void RegisterTabSpawnersBase(const TSharedRef<class FTabManager>& TabManager);
	virtual void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UObject* InObject);
	// Delegates
	void OnGraphEditorFocused(const TSharedRef<SGraphEditor>& InGraphEditor);
	/** Save the graph state for later editing */
	void SaveEditedObjectState();
	/** Restores the scenario graph we were editing or creates a new one if none is available */
	void RestoreObject();
	UObject* GetObjectToEdit() { return ObjectToEdit; }

	//~ Begin FEditorUndoClient Interface
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;
	// End of FEditorUndoClient

	//~ Begin FNotifyHook Interface
	virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;
	// End of FNotifyHook

	/** Spawns the tab with the update graph inside */
	TSharedRef<SWidget> SpawnProperties();

	virtual FName GetAppIdentifier() = 0;
	virtual TSharedRef<FApplicationMode> GetApplicationMode() = 0;
	virtual UGameFrameworkGraph* GetGraph() = 0;
	virtual void OnSetGraph(UGameFrameworkGraph* InGraph) = 0;
	virtual FName GetGraphName() const = 0;
	virtual TSubclassOf<class UEdGraph> GetGraphClass() = 0;
	virtual TSubclassOf<class UEdGraphSchema> GetSchemaClass() = 0;
	virtual TArray<FEditedDocumentInfo>& GetEditDocumentInfo() = 0;

protected:
	TSharedPtr<FDocumentTracker> DocumentManager;
	TWeakPtr<FDocumentTabFactory> GraphEditorTabFactoryPtr;
	/** The command list for this editor */
	TSharedPtr<FUICommandList> GraphEditorCommands;

	/* The Scenario being edited */
	UObject* ObjectToEdit;

	/** Property View */
	TSharedPtr<class IDetailsView> DetailsView;

	/** Currently focused graph */
	TWeakPtr<SGraphEditor> UpdateGraphEdPtr;

	/** Create widget for graph editing */
	TSharedRef<class SGraphEditor> CreateGraphEditorWidget(UEdGraph* InGraph);

private:
	void CreateCommandList();
	// Begin Delegates for graph editor commands
	void SelectAllNodes();
	bool CanSelectAllNodes() const;
	void CutSelectedNodes();
	bool CanCutNodes() const;
	void CopySelectedNodes();
	bool CanCopyNodes() const;
	void PasteSelectedNodes();
	bool CanPasteNodes() const;
	void DuplicateSelectedNodes();
	bool CanDuplicateNodes() const;
	void DeleteSelectedNodes();
	bool CanDeleteNodes() const;
	// End Delegates for graph editor commands
	/** Creates all internal widgets for the tabs to point at */
	void CreateInternalWidgets();
	FGraphPanelSelectionSet GetSelectedNodes() const;
	bool IsPropertyEditable() const;
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	bool InEditingMode(bool bGraphIsEditable) const;
	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);
	//~ Begin IToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	//~ End IToolkit Interface
	TSharedRef<FDocumentTabFactory> GetDocumentFactory();
};