// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GraphEditorModes.h"
#include "GameEditorTabs.h"
#include "GraphEditorTabFactory.h"
#include "GraphEditorBase.h"

/////////////////////////////////////////////////////
// FScenarioEditorApplicationMode

#define LOCTEXT_NAMESPACE "GraphApplicationMode"

FGraphEditorApplicationMode::FGraphEditorApplicationMode(TSharedPtr<class FGraphEditorBase> InEditor, FName InModeName, FText(*GetLocalizedMode)(const FName))
	: FApplicationMode(InModeName, GetLocalizedMode)
{
	Editor = InEditor;

	EditorTabFactories.RegisterFactory(MakeShareable(new FGraphDetailsSummoner(InEditor)));

	TabLayout = FTabManager::NewLayout("Standalone_Scenario_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(InEditor->GetToolbarTabId(), ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.7f)
					->AddTab(FGameEditorTabs::GraphEditorID, ETabState::ClosedTab)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.3f)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.6f)
						->AddTab(FGameEditorTabs::GraphDetailsID, ETabState::OpenedTab)
						->AddTab(FGameEditorTabs::SearchID, ETabState::ClosedTab)
					)
				)
			)
		);
}

void FGraphEditorApplicationMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	check(Editor.IsValid());

	// Mode-specific setup
	Editor.Pin()->PushTabFactories(EditorTabFactories);

	Editor.Pin()->RegisterTabSpawnersBase(InTabManager.ToSharedRef());
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FGraphEditorApplicationMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();

	check(Editor.IsValid());

	Editor.Pin()->SaveEditedObjectState();
}

void FGraphEditorApplicationMode::PostActivateMode()
{
	// Reopen any documents that were open when the blueprint was last saved
	check(Editor.IsValid());
	Editor.Pin()->RestoreObject();

	FApplicationMode::PostActivateMode();
}

#undef  LOCTEXT_NAMESPACE