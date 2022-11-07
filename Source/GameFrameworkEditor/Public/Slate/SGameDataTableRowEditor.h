// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Kismet2/StructureEditorUtils.h"
#include "DataTableEditorUtils.h"
#include "Misc/NotifyHook.h"
#include "Delegates/DelegateCombinations.h"
#include "Widgets/Input/SComboBox.h"

class IStructureDetailsView;
class SEditableTextBox;
class FStructOnScope;

DECLARE_DELEGATE_RetVal_OneParam(bool, FOnRowPreAdd, FName /*Row name*/);
DECLARE_DELEGATE_RetVal_OneParam(FName, FOnRowAdd, FName /*Row name*/);
DECLARE_DELEGATE_RetVal_OneParam(bool, FOnRowPreRemove, FName /*Row name*/);
DECLARE_DELEGATE_OneParam(FOnRowRemove, FName /*Row name*/);
DECLARE_DELEGATE_OneParam(FOnRowModified, FName /*Row name*/);
DECLARE_DELEGATE_OneParam(FOnRowSelected, FName /*Row name*/);

class GAMEFRAMEWORKEDITOR_API SGameDataTableRowEditor : public SCompoundWidget
	, public FNotifyHook
	, public FStructureEditorUtils::INotifyOnStructChanged
	, public FDataTableEditorUtils::INotifyOnDataTableChanged
{
public:
	SLATE_BEGIN_ARGS(SGameDataTableRowEditor) {}
	SLATE_END_ARGS()

	SGameDataTableRowEditor();
	virtual ~SGameDataTableRowEditor();

	TSharedPtr<FStructOnScope> GetCurrentRow();
	void MarkDatatableDirty();

	// FNotifyHook
	virtual void NotifyPreChange( FProperty* PropertyAboutToChange ) override;
	virtual void NotifyPostChange( const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged ) override;

	// INotifyOnStructChanged
	virtual void PreChange(const class UUserDefinedStruct* Struct, FStructureEditorUtils::EStructureEditorChangeInfo Info) override;
	virtual void PostChange(const class UUserDefinedStruct* Struct, FStructureEditorUtils::EStructureEditorChangeInfo Info) override;

	// INotifyOnDataTableChanged
	virtual void PreChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo Info) override;
	virtual void PostChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo Info) override;

	FOnRowPreAdd RowPreAddCallback;
	FOnRowAdd RowAddedCallback;
	FOnRowPreRemove RowPreRemoveCallback;
	FOnRowRemove RowRemovedCallback;
	FOnRowModified RowModifiedCallback;
	FOnRowSelected RowSelectedCallback;

private:

	TArray<TSharedPtr<FName>> CachedRowNames;
	TSharedPtr<FStructOnScope> CurrentRow;
	TSoftObjectPtr<UDataTable> DataTable; // weak obj ptr couldn't handle reimporting
	TSharedPtr<class IStructureDetailsView> StructureDetailsView;
	TSharedPtr<FName> SelectedName;
	TSharedPtr<SComboBox<TSharedPtr<FName>>> RowComboBox;
	TSharedPtr<SEditableTextBox> RenameTextBox;

	void RefreshNameList();
	void CleanBeforeChange();
	void Restore();

	UScriptStruct* GetScriptStruct() const;

	FName GetCurrentName() const;
	FText GetCurrentNameAsText() const;
	FString GetStructureDisplayName() const;
	TSharedRef<SWidget> OnGenerateWidget(TSharedPtr<FName> InItem);
	void OnSelectionChanged(TSharedPtr<FName> InItem, ESelectInfo::Type InSeletionInfo);

	FReply OnAddClicked();
	FReply OnRemoveClicked();
	FReply OnMoveRowClicked(FDataTableEditorUtils::ERowMoveDirection MoveDirection);
	FReply OnMoveToExtentClicked(FDataTableEditorUtils::ERowMoveDirection MoveDirection);
	void OnRowRenamed(const FText& Text, ETextCommit::Type CommitType);
	FReply OnResetToDefaultClicked();
	EVisibility GetResetToDefaultVisibility()const ;

public:

	void Construct(const FArguments& InArgs, UDataTable* Changed);

	void ChangeDataTable(UDataTable* Changed);

	void SelectRow(FName Name);

	void HandleUndoRedo();
};