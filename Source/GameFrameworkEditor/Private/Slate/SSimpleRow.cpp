// Fill out your copyright notice in the Description page of Project Settings.

#include "SSimpleRow.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SEditableLabel.h"
#include "GameFrameworkEditorCommands.h"
#include "SGameDataTableRowEditor.h"

#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Editor/UnrealEd/Classes/Factories/DataTableFactory.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "UnrealEd/Public/ObjectTools.h"
#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

namespace SimpleRowUI
{
	const FName IdColumnName(TEXT("Id"));
	const FName SimpleDescriptionColumnName(TEXT("简单描述"));
};

class SSingleTableRow : public SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>
{
public:
	SLATE_BEGIN_ARGS(SSingleTableRow) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& Args, const TSharedRef<STableViewBase>& OwnerTableView, TSharedPtr<FConfigTableRowWrapper> InPtr)
	{
		RowPtr = InPtr;

		SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>::Construct(
			FSuperRowType::FArguments()
			.Padding(1.0f),
			OwnerTableView
		);
	}

	/** Overridden from SMultiColumnTableRow.  Generates a widget for this column of the list view. */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
	{
        FConfigTableRowBase* RowData = (FConfigTableRowBase*)(RowPtr->ConfigTableRow);
		if (ColumnName == SimpleRowUI::IdColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(RowData->GetUniqueId())));
		}
		else if (ColumnName == SimpleRowUI::SimpleDescriptionColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(RowData->GetSimpleDescription()));
		}

		return SNullWidget::NullWidget;
	}

private:

	TSharedPtr<FConfigTableRowWrapper> RowPtr;
};

void SSimpleRow::Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList) {
	ConstructWidgetSources();

	ChildSlot
		[
			ConstructPage()
		];
}

SSimpleRow::~SSimpleRow() {
	
}

void SSimpleRow::ConstructWidgetSources() {
	Source = GetRowSource();
}

bool SSimpleRow::AllRowModified(FName Name, struct FConfigTableRowBase* ChangeRow, struct FConfigTableRowBase* OriginRow) {
    return true;
}

TSharedRef<SWidget> SSimpleRow::ConstructPage() {
	return SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SAssignNew(ListView, SListView<TSharedPtr<FConfigTableRowWrapper>>)
				.ListItemsSource(&Source)
				.SelectionMode(ESelectionMode::Single)
				.OnGenerateRow(this, &SSimpleRow::ListViewOnGenerateRow)
				.OnSelectionChanged(this, &SSimpleRow::OnSelectionChanged)
				.HeaderRow
				(
                    ConstructListViewHeadRow()
				)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				ConstructRowEditor()
			]
		];
}

TSharedRef<SWidget> SSimpleRow::ConstructRowEditor() {
    auto RowDataTable = GetDataTable();
	EditorRow = SNew(SGameDataTableRowEditor, RowDataTable);
	EditorRow->RowAddedCallback.BindSP(this, &SSimpleRow::TableRowAdded);
	EditorRow->RowPreRemoveCallback.BindSP(this, &SSimpleRow::TableRowPreRemove);
	EditorRow->RowRemovedCallback.BindSP(this, &SSimpleRow::TableRowRemoved);
	EditorRow->RowModifiedCallback.BindSP(this, &SSimpleRow::TableRowModified);
	EditorRow->RowSelectedCallback.BindSP(this, &SSimpleRow::TableSetHighlightedRow);

	return EditorRow.ToSharedRef();
}

FName SSimpleRow::TableRowAdded(FName Name) {
    auto RowDataTable = GetDataTable();
    if (RowDataTable != nullptr) {
        FName NewRowName = NewRowInit(Name);
        if (!NewRowName.IsNone()) {
            FConfigTableRowBase* FindRow = RowDataTable->FindRow<FConfigTableRowBase>(NewRowName, "");

            auto TableUsingStruct = RowDataTable->GetRowStruct();
            int32 StructureSize = TableUsingStruct->GetStructureSize();
            FMoneyTypeConfigTableRow* NewRawRowData = (FMoneyTypeConfigTableRow*)FMemory::Malloc(StructureSize);
            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, FindRow);
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = (uint8*)NewRawRowData;

            Source.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));

            ListView->RebuildList();
            ListView->ScrollToBottom();
            return NewRowName;
        }
        else {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("创建新行错误")), 5.0);
            return FName();
        }
    }
    return Name;
}

bool SSimpleRow::TableRowPreRemove(FName Name) {
    for (auto Index = 0; Index < Source.Num(); ++Index) {
        FConfigTableRowBase* RowData = (FConfigTableRowBase*)(Source[Index]->ConfigTableRow);
        FName RowName = *FString::FromInt(RowData->GetUniqueId());
        if (RowName == Name) {
            return AllowRowRemove(RowData);
        }
    }
    return true;
}

void SSimpleRow::TableRowRemoved(FName Name) {
    for (auto Index = 0; Index < Source.Num(); ++Index) {
        FConfigTableRowBase* RowData = (FConfigTableRowBase*)(Source[Index]->ConfigTableRow);
        FName RowName = *FString::FromInt(RowData->GetUniqueId());
        if (RowName == Name) {
            RowRemoved(RowData);
            Source.RemoveAt(Index);
            ListView->RebuildList();
            break;
        }
    }
}

void SSimpleRow::TableRowModified(FName Name) {
    auto RowDataTable = GetDataTable();
    if (RowDataTable != nullptr) {
        FConfigTableRowBase* FindRow = (FConfigTableRowBase*)RowDataTable->FindRowUnchecked(Name);
        for (auto Index = 0; Index < Source.Num(); ++Index) {
            FConfigTableRowBase* RowData = (FConfigTableRowBase*)(Source[Index]->ConfigTableRow);
            FName RowIdName = *FString::FromInt(RowData->GetUniqueId());
            if (FindRow->GetUniqueId() == RowData->GetUniqueId() || RowIdName == Name) {
                auto TableUsingStruct = RowDataTable->GetRowStruct();
                if (AllRowModified(Name, FindRow, RowData)) {
                    TableUsingStruct->CopyScriptStruct(RowData, FindRow);
                }
                else {
                    TableUsingStruct->CopyScriptStruct(FindRow, RowData);
                }

                ListView->RebuildList();
                break;
            }
        }
    }
}

void SSimpleRow::TableSetHighlightedRow(FName Name) {
	if (Name == HighlightedRowName)
	{
		return;
	}

	if (Name.IsNone())
	{
		HighlightedRowName = NAME_None;

		// Synchronize the list views
		ListView->ClearSelection();
	}
	else
	{
		TSharedPtr<FConfigTableRowWrapper>* NewSelectionPtr = Source.FindByPredicate([&Name](const TSharedPtr<FConfigTableRowWrapper>& RowDataWrapper) -> bool
		{
            FConfigTableRowBase* RowData = (FConfigTableRowBase*)(RowDataWrapper->ConfigTableRow);
			return *FString::FromInt(RowData->GetUniqueId()) == Name;
		});
		
		// Synchronize the list views
		if (NewSelectionPtr)
		{
			HighlightedRowName = Name;
			ListView->SetSelection(*NewSelectionPtr);
		}
		else
		{
			ListView->ClearSelection();
		}
	}
}

void SSimpleRow::OnSelectionChanged(TSharedPtr<FConfigTableRowWrapper> InNewSelection, ESelectInfo::Type InSelectInfo) {
	if (InNewSelection.IsValid()) {
        FConfigTableRowBase* RowData = (FConfigTableRowBase*)(InNewSelection->ConfigTableRow);
		FName RowName = *FString::FromInt(RowData->GetUniqueId());
		const bool bSelectionChanged = !InNewSelection.IsValid() || RowName != HighlightedRowName;
		const FName NewRowName = (InNewSelection.IsValid()) ? RowName : NAME_None;

		TableSetHighlightedRow(NewRowName);

		if (bSelectionChanged)
		{
			EditorRow->SelectRow(HighlightedRowName);
		}
	}
	else {
		ListView->ClearSelection();
	}
}

TSharedRef<ITableRow> SSimpleRow::ListViewOnGenerateRow(TSharedPtr<FConfigTableRowWrapper> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SSingleTableRow, OwnerTable, Item);
}

TSharedRef<class SHeaderRow> SSimpleRow::ConstructListViewHeadRow() {
    return SNew(SHeaderRow)
        + SHeaderRow::Column(SimpleRowUI::IdColumnName)
        .DefaultLabel(FText::FromName(SimpleRowUI::IdColumnName))
        .FillWidth(33.0f)
        + SHeaderRow::Column(SimpleRowUI::SimpleDescriptionColumnName)
        .DefaultLabel(FText::FromName(SimpleRowUI::SimpleDescriptionColumnName))
        .FillWidth(53.0f);
}

bool SSimpleRow::AllowRowRemove(struct FConfigTableRowBase* RemoveRow) {
    return true;
}

void SSimpleRow::RowRemoved(struct FConfigTableRowBase* RemoveRow) {

}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION