// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Docking/TabManager.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
#include "WorkflowOrientedApp/ApplicationMode.h"

/** Application mode for main scenario editing mode */
class FGraphEditorApplicationMode : public FApplicationMode
{
public:
	FGraphEditorApplicationMode(TSharedPtr<class FGraphEditorBase> InEditor, FName InModeName, FText(*GetLocalizedMode)(const FName));

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<class FGraphEditorBase> Editor;

	// Set of spawnable tabs in scenario editing mode
	FWorkflowAllowedTabSet EditorTabFactories;
};