// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SGameDataTableRowEditor.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"
#include "UObject/StructOnScope.h"

#include "PropertyEditorModule.h"
#include "IStructureDetailsView.h"
#include "IDetailsView.h"

#define LOCTEXT_NAMESPACE "SGameDataTableRowEditor"

class FStructFromDataTable : public FStructOnScope
{
	TWeakObjectPtr<UDataTable> DataTable;
	FName RowName;

public:
	FStructFromDataTable(UDataTable* InDataTable, FName InRowName)
		: FStructOnScope()
		, DataTable(InDataTable)
		, RowName(InRowName)
	{}

	virtual uint8* GetStructMemory() override
	{
		return (DataTable.IsValid() && !RowName.IsNone()) ? DataTable->FindRowUnchecked(RowName) : nullptr;
	}

	virtual const uint8* GetStructMemory() const override
	{
		return (DataTable.IsValid() && !RowName.IsNone()) ? DataTable->FindRowUnchecked(RowName) : nullptr;
	}

	virtual const UScriptStruct* GetStruct() const override
	{
		return DataTable.IsValid() ? DataTable->GetRowStruct() : nullptr;
	}

	virtual UPackage* GetPackage() const override
	{
		return DataTable.IsValid() ? DataTable->GetOutermost() : nullptr;
	}

	virtual void SetPackage(UPackage* InPackage) override
	{
	}

	virtual bool IsValid() const override
	{
		return !RowName.IsNone()
			&& DataTable.IsValid()
			&& DataTable->GetRowStruct()
			&& DataTable->FindRowUnchecked(RowName);
	}

	virtual void Destroy() override
	{
		DataTable = nullptr;
		RowName = NAME_None;
	}

	FName GetRowName() const
	{
		return RowName;
	}
};

SGameDataTableRowEditor::SGameDataTableRowEditor() 
	: SCompoundWidget()
{
}

SGameDataTableRowEditor::~SGameDataTableRowEditor()
{
}

TSharedPtr<FStructOnScope> SGameDataTableRowEditor::GetCurrentRow()
{
	return CurrentRow;
}

void SGameDataTableRowEditor::MarkDatatableDirty()
{
	check(DataTable.IsValid());

	FDataTableEditorUtils::BroadcastPostChange(DataTable.Get(), FDataTableEditorUtils::EDataTableChangeInfo::RowData);

	DataTable->MarkPackageDirty();

	if (SelectedName.IsValid() && *SelectedName != NAME_None) {
		RowModifiedCallback.ExecuteIfBound(*SelectedName);
	}
}

void SGameDataTableRowEditor::NotifyPreChange(FProperty* PropertyAboutToChange )
{
	check(DataTable.IsValid());

	DataTable->Modify();

	FDataTableEditorUtils::BroadcastPreChange(DataTable.Get(), FDataTableEditorUtils::EDataTableChangeInfo::RowData);
}

void SGameDataTableRowEditor::NotifyPostChange( const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged )
{
	check(DataTable.IsValid());

	FDataTableEditorUtils::BroadcastPostChange(DataTable.Get(), FDataTableEditorUtils::EDataTableChangeInfo::RowData);

	DataTable->MarkPackageDirty();

	if (SelectedName.IsValid() && *SelectedName != NAME_None) {
		RowModifiedCallback.ExecuteIfBound(*SelectedName);
	}
}

void SGameDataTableRowEditor::PreChange(const class UUserDefinedStruct* Struct, FStructureEditorUtils::EStructureEditorChangeInfo Info)
{
	if (Struct && (GetScriptStruct() == Struct))
	{
		CleanBeforeChange();
	}
}

void SGameDataTableRowEditor::PostChange(const class UUserDefinedStruct* Struct, FStructureEditorUtils::EStructureEditorChangeInfo Info)
{
	if (Struct && (GetScriptStruct() == Struct))
	{
		Restore();
	}
}

void SGameDataTableRowEditor::PreChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo Info)
{
	if ((Changed == DataTable.Get()) && (FDataTableEditorUtils::EDataTableChangeInfo::RowList == Info))
	{
		CleanBeforeChange();
	}
}

void SGameDataTableRowEditor::PostChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo Info)
{
	FSoftObjectPath::InvalidateTag(); // Should be removed after UE-5615 is fixed
	if ((Changed == DataTable.Get()) && (FDataTableEditorUtils::EDataTableChangeInfo::RowList == Info))
	{
		RefreshNameList();
		Restore();
	}
}

void SGameDataTableRowEditor::CleanBeforeChange()
{
	if (StructureDetailsView.IsValid())
	{
		StructureDetailsView->SetStructureData(NULL);
	}
	if (CurrentRow.IsValid())
	{
		CurrentRow->Destroy();
		CurrentRow.Reset();
	}
}

void SGameDataTableRowEditor::RefreshNameList()
{
	CachedRowNames.Empty();
	if (DataTable.IsValid())
	{
		auto RowNames = DataTable->GetRowNames();
		for (auto RowName : RowNames)
		{
			CachedRowNames.Add(MakeShareable(new FName(RowName)));
		}
	}
}

void SGameDataTableRowEditor::Restore()
{
	if (!SelectedName.IsValid() || !SelectedName->IsNone())
	{
		if (SelectedName.IsValid())
		{
			auto CurrentName = *SelectedName;
			SelectedName = NULL;
			for (auto Element : CachedRowNames)
			{
				if (*Element == CurrentName)
				{
					SelectedName = Element;
					break;
				}
			}
		}

		if (!SelectedName.IsValid() && CachedRowNames.Num() && CachedRowNames[0].IsValid())
		{
			SelectedName = CachedRowNames[0];
		}

		if (RowComboBox.IsValid())
		{
			RowComboBox->SetSelectedItem(SelectedName);
		}
	}
	else
	{
		if (RowComboBox.IsValid())
		{
			RowComboBox->ClearSelection();
		}
	}

	auto FinalName = SelectedName.IsValid() ? *SelectedName : NAME_None;
	CurrentRow = MakeShareable(new FStructFromDataTable(DataTable.Get(), FinalName));
	if (StructureDetailsView.IsValid())
	{
		StructureDetailsView->SetStructureData(CurrentRow);
	}

	RowSelectedCallback.ExecuteIfBound(FinalName);
}

UScriptStruct* SGameDataTableRowEditor::GetScriptStruct() const
{
	return DataTable.IsValid() ? DataTable->RowStruct : NULL;
}

FName SGameDataTableRowEditor::GetCurrentName() const
{
	return SelectedName.IsValid() ? *SelectedName : NAME_None;
}

FText SGameDataTableRowEditor::GetCurrentNameAsText() const
{
	return FText::FromName(GetCurrentName());
}

FString SGameDataTableRowEditor::GetStructureDisplayName() const
{
	const auto Struct = DataTable.IsValid() ? DataTable->RowStruct : NULL;
	return Struct 
		? Struct->GetDisplayNameText().ToString()
		: LOCTEXT("Error_UnknownStruct", "Error: Unknown Struct").ToString();
}

TSharedRef<SWidget> SGameDataTableRowEditor::OnGenerateWidget(TSharedPtr<FName> InItem)
{
	return SNew(STextBlock).Text(FText::FromName(InItem.IsValid() ? *InItem : NAME_None));
}

void SGameDataTableRowEditor::OnSelectionChanged(TSharedPtr<FName> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InItem.IsValid() && InItem != SelectedName)
	{
		CleanBeforeChange();

		SelectedName = InItem;

		Restore();

		if (RenameTextBox.IsValid())
		{
			// refresh name, in case of a pending rename action
			RenameTextBox->SetText(TAttribute<FText>(this, &SGameDataTableRowEditor::GetCurrentNameAsText));
		}
	}
}

void SGameDataTableRowEditor::ChangeDataTable(UDataTable* Changed) {
	DataTable = Changed;
	RefreshNameList();
	Restore();
}

void SGameDataTableRowEditor::SelectRow(FName InName)
{
	TSharedPtr<FName> NewSelectedName;
	for (auto Name : CachedRowNames)
	{
		if (Name.IsValid() && (*Name == InName))
		{
			NewSelectedName = Name;
		}
	}
	if (!NewSelectedName.IsValid())
	{
		NewSelectedName = MakeShareable(new FName(InName));
	}
	OnSelectionChanged(NewSelectedName, ESelectInfo::Direct);
}

void SGameDataTableRowEditor::HandleUndoRedo()
{
	RefreshNameList();
	Restore();
}

FReply SGameDataTableRowEditor::OnAddClicked()
{
	if (DataTable.IsValid())
	{
		FName NewName = DataTableUtils::MakeValidName(TEXT("NewRow"));
		const TArray<FName> ExisitngNames = DataTable->GetRowNames();
		while (ExisitngNames.Contains(NewName))
		{
			NewName.SetNumber(NewName.GetNumber() + 1);
		}
		if (RowPreAddCallback.IsBound()) {
			if (!RowPreAddCallback.Execute(NewName)) {
				return FReply::Unhandled();
			}
		}
		FDataTableEditorUtils::AddRow(DataTable.Get(), NewName);
		if (RowAddedCallback.IsBound()) {
			FName executeNewName = RowAddedCallback.Execute(NewName);
			if (executeNewName.IsValid()) {
				SelectRow(executeNewName);
			}
		}
		else {
			SelectRow(NewName);
		}
	}
	return FReply::Handled();
}

FReply SGameDataTableRowEditor::OnRemoveClicked()
{
	if (DataTable.IsValid())
	{
		const FName RowToRemove = GetCurrentName();
		const int32 RowToRemoveIndex = CachedRowNames.IndexOfByPredicate([&](const TSharedPtr<FName>& InRowName) -> bool
		{
			return *InRowName == RowToRemove;
		});

		if (RowPreRemoveCallback.IsBound()) {
			if (!RowPreRemoveCallback.Execute(RowToRemove)) {
				return FReply::Unhandled();
			}
		}

		if (FDataTableEditorUtils::RemoveRow(DataTable.Get(), RowToRemove))
		{
			RowRemovedCallback.ExecuteIfBound(RowToRemove);
			// Try and keep the same Row Index selected
			const int32 RowIndexToSelect = FMath::Clamp(RowToRemoveIndex, 0, CachedRowNames.Num() - 1);
			if (CachedRowNames.IsValidIndex(RowIndexToSelect))
			{
				SelectRow(*CachedRowNames[RowIndexToSelect]);
			}
		}
	}
	return FReply::Handled();
}

FReply SGameDataTableRowEditor::OnMoveRowClicked(FDataTableEditorUtils::ERowMoveDirection MoveDirection)
{
	if (DataTable.IsValid())
	{
		const FName RowToMove = GetCurrentName();
		FDataTableEditorUtils::MoveRow(DataTable.Get(), RowToMove, MoveDirection);
	}
	return FReply::Handled();
}

FReply SGameDataTableRowEditor::OnMoveToExtentClicked(FDataTableEditorUtils::ERowMoveDirection MoveDirection)
{
	if (DataTable.IsValid())
	{
		// We move by the Row map size, as FDataTableEditorUtils::MoveRow will automatically clamp this as appropriate
		const FName RowToMove = GetCurrentName();
		FDataTableEditorUtils::MoveRow(DataTable.Get(), RowToMove, MoveDirection, DataTable->GetRowMap().Num());
	}
	return FReply::Handled();
}

void SGameDataTableRowEditor::OnRowRenamed(const FText& Text, ETextCommit::Type CommitType)
{
	if (!GetCurrentNameAsText().EqualTo(Text) && DataTable.IsValid())
	{
		const FName NewName = DataTableUtils::MakeValidName(Text.ToString());
		for (auto Name : CachedRowNames)
		{
			if (Name.IsValid() && (*Name == NewName))
			{
				 //the name already exists
				return;
			}
		}

		const FName OldName = GetCurrentName();
		FDataTableEditorUtils::RenameRow(DataTable.Get(), OldName, NewName);
		SelectRow(NewName);
	}
}

FReply SGameDataTableRowEditor::OnResetToDefaultClicked()
{
	if (DataTable.IsValid() && SelectedName.IsValid())
	{
		FDataTableEditorUtils::ResetToDefault(DataTable.Get(), *SelectedName.Get());
	}

	return FReply::Handled();
}

EVisibility SGameDataTableRowEditor::GetResetToDefaultVisibility() const
{
	EVisibility VisibleState = EVisibility::Collapsed;

	if (DataTable.IsValid() && SelectedName.IsValid())
	{
		if (FDataTableEditorUtils::DiffersFromDefault(DataTable.Get(), *SelectedName.Get()))
		{
			VisibleState = EVisibility::Visible;
		}
	}

	return VisibleState;
}

void SGameDataTableRowEditor::Construct(const FArguments& InArgs, UDataTable* Changed)
{
	DataTable = Changed;
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		FDetailsViewArgs ViewArgs;
		ViewArgs.bAllowSearch = false;
		ViewArgs.bHideSelectionTip = false;
#if ENGINE_MAJOR_VERSION > 4
		ViewArgs.bShowObjectLabel = false;
#else
		ViewArgs.bShowActorLabel = false;		
#endif
		ViewArgs.NotifyHook = this;

		FStructureDetailsViewArgs StructureViewArgs;
		StructureViewArgs.bShowObjects = false;
		StructureViewArgs.bShowAssets = true;
		StructureViewArgs.bShowClasses = true;
		StructureViewArgs.bShowInterfaces = false;

		StructureDetailsView = PropertyModule.CreateStructureDetailView(ViewArgs, StructureViewArgs, CurrentRow, LOCTEXT("RowValue", "Row Value"));
	}

	RefreshNameList();
	Restore();
	const float ButtonWidth = 85.0f;
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SButton)
#if ENGINE_MAJOR_VERSION > 4
				.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
#else
				.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
#endif
				.ForegroundColor(FSlateColor::UseForeground())
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SGameDataTableRowEditor::OnAddClicked)
				.ToolTipText(LOCTEXT("AddRowTooltip", "Add a new Row to the data table"))
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("+"))
				]
				[
					SNew(SImage)
#if ENGINE_MAJOR_VERSION > 4
					.Image(FAppStyle::Get().GetBrush("Plus"))
#else
					.Image(FEditorStyle::Get().GetBrush("Plus"))
#endif
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SButton)
#if ENGINE_MAJOR_VERSION > 4
				.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
#else
				.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
#endif
				.ForegroundColor(FSlateColor::UseForeground())
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SGameDataTableRowEditor::OnRemoveClicked)
				.ToolTipText(LOCTEXT("RemoveRowTooltip", "Remove the currently selected Row from the data table"))
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("-"))
				]
				[
					SNew(SImage)
#if ENGINE_MAJOR_VERSION > 4
					.Image(FAppStyle::Get().GetBrush("Cross"))
#else
					.Image(FEditorStyle::Get().GetBrush("Cross"))
#endif
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SBox)
				.WidthOverride(2 * ButtonWidth)
				.ToolTipText(LOCTEXT("SelectedRowTooltip", "Select a Row to edit"))
				[
					SAssignNew(RowComboBox, SComboBox<TSharedPtr<FName>>)
					.OptionsSource(&CachedRowNames)
					.OnSelectionChanged(this, &SGameDataTableRowEditor::OnSelectionChanged)
					.OnGenerateWidget(this, &SGameDataTableRowEditor::OnGenerateWidget)
					.Content()
					[
						SNew(STextBlock).Text(this, &SGameDataTableRowEditor::GetCurrentNameAsText)
					]
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SButton)
				.OnClicked(this, &SGameDataTableRowEditor::OnResetToDefaultClicked)
				.Visibility(this, &SGameDataTableRowEditor::GetResetToDefaultVisibility)
				.ContentPadding(FMargin(5.f, 0.f))
				.ToolTipText(LOCTEXT("ResetToDefaultToolTip", "Reset to Default"))
#if ENGINE_MAJOR_VERSION > 4
				.ButtonStyle(FAppStyle::Get(), "NoBorder")
#else
				.ButtonStyle(FEditorStyle::Get(), "NoBorder")
#endif
				.ForegroundColor(FSlateColor::UseForeground())
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Content()
				[
					SNew(SImage)
#if ENGINE_MAJOR_VERSION > 4
					.Image(FAppStyle::GetBrush("PropertyWindow.DiffersFromDefault"))
#else
					.Image(FEditorStyle::GetBrush("PropertyWindow.DiffersFromDefault"))
#endif
				]
			]

			+ SHorizontalBox::Slot()
			[
				SNew(SSpacer)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(SBox)
				.HAlign(HAlign_Right)
				[
					SNew(STextBlock).Text(LOCTEXT("RowNameLabel", "Row Name:"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SBox)
				.WidthOverride(2 * ButtonWidth)
				[
					SAssignNew(RenameTextBox, SEditableTextBox)
					.Text(this, &SGameDataTableRowEditor::GetCurrentNameAsText)
					.OnTextCommitted(this, &SGameDataTableRowEditor::OnRowRenamed)
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SButton)
#if ENGINE_MAJOR_VERSION > 4
				.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
#else
				.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
#endif
				.ForegroundColor(FSlateColor::UseForeground())
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SGameDataTableRowEditor::OnMoveRowClicked, FDataTableEditorUtils::ERowMoveDirection::Up)
				.ToolTipText(LOCTEXT("MoveUpTooltip", "Move the currently selected Row up by one in the data table"))
				[
					SNew(STextBlock)
#if ENGINE_MAJOR_VERSION > 4
					.Font(FAppStyle::Get().GetFontStyle("FontAwesome.14"))
#else
					.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.14"))
#endif
					.Text(FText::FromString(FString(TEXT("\xf106"))) /*fa-angle-up*/)
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SButton)
#if ENGINE_MAJOR_VERSION > 4
				.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
#else
				.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
#endif
				.ForegroundColor(FSlateColor::UseForeground())
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SGameDataTableRowEditor::OnMoveRowClicked, FDataTableEditorUtils::ERowMoveDirection::Down)
				.ToolTipText(LOCTEXT("MoveDownTooltip", "Move the currently selected Row down by one in the data table"))
				[
					SNew(STextBlock)
#if ENGINE_MAJOR_VERSION > 4
					.Font(FAppStyle::Get().GetFontStyle("FontAwesome.14"))
#else
					.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.14"))
#endif
					.Text(FText::FromString(FString(TEXT("\xf107"))) /*fa-angle-down*/)
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SButton)
#if ENGINE_MAJOR_VERSION > 4
				.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
#else
				.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
#endif
				.ForegroundColor(FSlateColor::UseForeground())
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SGameDataTableRowEditor::OnMoveToExtentClicked, FDataTableEditorUtils::ERowMoveDirection::Up)
				.ToolTipText(LOCTEXT("MoveToTopTooltip", "Move the currently selected Row to the top of the data table"))
				[
					SNew(STextBlock)
#if ENGINE_MAJOR_VERSION > 4
					.Font(FAppStyle::Get().GetFontStyle("FontAwesome.14"))
#else
					.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.14"))
#endif
					.Text(FText::FromString(FString(TEXT("\xf102"))) /*fa-angle-double-up*/)
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SButton)
#if ENGINE_MAJOR_VERSION > 4
				.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
#else
				.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
#endif
				.ForegroundColor(FSlateColor::UseForeground())
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SGameDataTableRowEditor::OnMoveToExtentClicked, FDataTableEditorUtils::ERowMoveDirection::Down)
				.ToolTipText(LOCTEXT("MoveToBottomTooltip", "Move the currently selected Row to the bottom of the data table"))
				[
					SNew(STextBlock)
#if ENGINE_MAJOR_VERSION > 4
					.Font(FAppStyle::Get().GetFontStyle("FontAwesome.14"))
#else
					.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.14"))
#endif
					.Text(FText::FromString(FString(TEXT("\xf103"))) /*fa-angle-double-down*/)
				]
			]
		]
		+ SVerticalBox::Slot()
		[
			StructureDetailsView->GetWidget().ToSharedRef()
		]
	];
}

#undef LOCTEXT_NAMESPACE
