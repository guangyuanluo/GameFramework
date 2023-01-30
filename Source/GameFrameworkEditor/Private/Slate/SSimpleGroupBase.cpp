// Fill out your copyright notice in the Description page of Project Settings.

#include "SSimpleGroupBase.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SEditableLabel.h"
#include "GameFrameworkEditorCommands.h"
#include "Slate/SGameDataTableRowEditor.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Editor/UnrealEd/Classes/Factories/DataTableFactory.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "ObjectTools.h"
#include "GameFrameworkEditor/Public/GameFrameworkEditorWidgetTool.h"
#include "GameFrameworkEditor/Public/GameFrameworkEditor.h"
#include "Base/ConfigTable/SimpleGroupConfigTableRowBase.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

namespace GroupUI
{
	const FName GroupIdColumnName(TEXT("组id"));
	const FName GroupDescriptionColumnName(TEXT("组描述"));
	const FName GroupEditColumnName(TEXT("组配置"));
};

class SGroupRow : public SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>
{
public:
	SLATE_BEGIN_ARGS(SGroupRow) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& Args, const TSharedRef<STableViewBase>& OwnerTableView, TSharedPtr<FConfigTableRowWrapper> InPtr, SSimpleGroupBase* parentWindow, TArray<TSharedPtr<FConfigTableRowWrapper>>* GroupSource, UDataTable* DataTable)
	{
		GroupPtr = InPtr;
		ParentWindow = parentWindow;
		mGroupSource = GroupSource;
		mDataTable = DataTable;

		SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>::Construct(
			FSuperRowType::FArguments()
			.Padding(1.0f),
			OwnerTableView
		);
	}

	/** Overridden from SMultiColumnTableRow.  Generates a widget for this column of the list view. */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
	{
        FConfigTableRowBase* RowData = (FConfigTableRowBase*)(GroupPtr->ConfigTableRow);
		if (ColumnName == GroupUI::GroupIdColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(RowData->GetUniqueId())));
		}
		else if (ColumnName == GroupUI::GroupDescriptionColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(RowData->GetSimpleDescription()));
		}
		else if (ColumnName == GroupUI::GroupEditColumnName) {
			return SNew(SButton)
				.ContentPadding(-3)
				.OnClicked(this, &SGroupRow::EditGroupTableButtonClicked)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("配置组成员")))
				];
		}

		return SNullWidget::NullWidget;
	}

private:

	TSharedPtr<FConfigTableRowWrapper> GroupPtr;
	SSimpleGroupBase* ParentWindow;
	TArray<TSharedPtr<FConfigTableRowWrapper>>* mGroupSource;
	UDataTable* mDataTable;
    FSimpleGroupConfigTableRowBase* PoolRow;

	TArray<int>* mGroupItemArray;
	TSharedPtr<FConfigTableRowWrapper> mUnselectListSelectItem;
	TSharedPtr<FConfigTableRowWrapper> mSelectListSelectItem;
	TArray<TSharedPtr<FConfigTableRowWrapper>> mUnselectListSource;
	TArray<TSharedPtr<FConfigTableRowWrapper>> mSelectListSource;

	FReply EditGroupTableButtonClicked() {
		const FVector2D BrowserWindowSize(800, 600);
		
		FSlateApplication::Get().AddModalWindow(
			SNew(SWindow)
			.Title(FText::FromString(TEXT("组成员配置")))
			.HasCloseButton(true)
			.ClientSize(BrowserWindowSize)
			[
				ConstructEditGroupPage()
			]
		, ParentWindow->AsShared());
		
		return FReply::Handled();
	}
	
	TSharedRef<SWidget> ConstructEditGroupPage() {
		TSharedPtr<SHorizontalBox> pageButtons = SNew(SHorizontalBox);
		TSharedPtr<SVerticalBox> page = SNew(SVerticalBox);

        FConfigTableRowBase* RowData = (FConfigTableRowBase*)(GroupPtr->ConfigTableRow);
		PoolRow = mDataTable->FindRow<FSimpleGroupConfigTableRowBase>(*FString::FromInt(RowData->GetUniqueId()), "");

		//组
        GeneratePageItem(pageButtons, page, &PoolRow->GetGroupItems());

		return SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.FillHeight(0.05f)
			[
				pageButtons->AsShared()
			]
			+ SVerticalBox::Slot()
			.FillHeight(0.95f)
			[
				page->AsShared()
			]
		];
	}

	void GeneratePageItem(TSharedPtr<SHorizontalBox> pageButtons, TSharedPtr<SVerticalBox> page, TArray<int>* GroupArray) {
        mGroupItemArray = GroupArray;

		page->ClearChildren();

		mUnselectListSelectItem.Reset();
		mSelectListSelectItem.Reset();
		mUnselectListSource.Empty();
		mSelectListSource.Empty();

        auto GroupItemSource = ParentWindow->GetGroupItemSource();
		for (int Index = 0; Index < GroupItemSource.Num(); ++Index) {
            FConfigTableRowBase* RowData = (FConfigTableRowBase*)(GroupPtr->ConfigTableRow);
            FConfigTableRowBase* GroupItemRow = (FConfigTableRowBase*)(GroupItemSource[Index]->ConfigTableRow);
			if (mGroupItemArray->Contains(GroupItemRow->GetUniqueId())) {
				mSelectListSource.Add(GroupItemSource[Index]);
			}
			else {
				mUnselectListSource.Add(GroupItemSource[Index]);
			}
		}

		TSharedPtr<SListView<TSharedPtr<FConfigTableRowWrapper>>> unSelectList = SNew(SListView<TSharedPtr<FConfigTableRowWrapper>>)
			.SelectionMode(ESelectionMode::Single)
			.ListItemsSource(&mUnselectListSource)
			.OnGenerateRow_Lambda([this](TSharedPtr<FConfigTableRowWrapper> Entry, const TSharedRef<STableViewBase>& OwnerTable) -> TSharedRef<ITableRow>
			{
            FConfigTableRowBase* EntryRow = (FConfigTableRowBase*)(Entry->ConfigTableRow);
				return SNew(STableRow<TSharedPtr<FName>>, OwnerTable)
				[
					SNew(STextBlock).Text(FText::FromString(EntryRow->GetSimpleDescription()))
				];
			})
			.OnSelectionChanged_Lambda([this](TSharedPtr<FConfigTableRowWrapper> InNewSelection, ESelectInfo::Type InSelectInfo)
			{
				mUnselectListSelectItem = InNewSelection;
			});
		TSharedPtr<SListView<TSharedPtr<FConfigTableRowWrapper>>> SelectList = SNew(SListView<TSharedPtr<FConfigTableRowWrapper>>)
			.SelectionMode(ESelectionMode::Single)
			.ListItemsSource(&mSelectListSource)
			.OnGenerateRow_Lambda([this](TSharedPtr<FConfigTableRowWrapper> Entry, const TSharedRef<STableViewBase>& OwnerTable) -> TSharedRef<ITableRow>
			{
            FConfigTableRowBase* EntryRow = (FConfigTableRowBase*)(Entry->ConfigTableRow);
				return SNew(STableRow<TSharedPtr<FName>>, OwnerTable)
				[
					SNew(STextBlock).Text(FText::FromString(EntryRow->GetSimpleDescription()))
				];
			})
			.OnSelectionChanged_Lambda([this](TSharedPtr<FConfigTableRowWrapper> InNewSelection, ESelectInfo::Type InSelectInfo)
			{
				mSelectListSelectItem = InNewSelection;
			});

		page->AddSlot()
		[
			SNew(SBox)
			.Padding(FMargin(15, 0))
			.Content()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.4)
				[
					unSelectList->AsShared()
				]
				+ SHorizontalBox::Slot()
				.Padding(10.0f, 200.0f)
				.FillWidth(0.2)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					[
						SNew(SButton)
						.OnClicked_Lambda([this, GroupItemSource, SelectList, unSelectList]() {
							mGroupItemArray->Empty();
							mUnselectListSelectItem.Reset();
							mSelectListSelectItem.Reset();
							mUnselectListSource.Empty();
							mSelectListSource.Empty();

							for (int Index = 0; Index < GroupItemSource.Num(); ++Index) {
                                FConfigTableRowBase* GroupItemRow = (FConfigTableRowBase*)(GroupItemSource[Index]->ConfigTableRow);
								mGroupItemArray->Add(GroupItemRow->GetUniqueId());
								mSelectListSource.Add(GroupItemSource[Index]);
							}

							SelectList->RebuildList();
							unSelectList->RebuildList();

							mDataTable->MarkPackageDirty();

							return FReply::Handled();
						})
						.Content()
						[
							SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.Text(FText::FromString(TEXT("全选")))
						]
					]
					+ SVerticalBox::Slot()
					[
						SNew(SButton)
						.OnClicked_Lambda([this, GroupItemSource, SelectList, unSelectList]() {
							if (mUnselectListSelectItem.IsValid()) {
                                FConfigTableRowBase* UnselectHeroListSelectRow = (FConfigTableRowBase*)(mUnselectListSelectItem->ConfigTableRow);
								if (!mGroupItemArray->Contains(UnselectHeroListSelectRow->GetUniqueId())) {
									for (int Index = 0; Index < GroupItemSource.Num(); ++Index) {
                                        FConfigTableRowBase* GroupItemRow = (FConfigTableRowBase*)(GroupItemSource[Index]->ConfigTableRow);
										if (GroupItemRow->GetUniqueId() == UnselectHeroListSelectRow->GetUniqueId()) {
											{
												bool insertSuccess = false;
												for (int ListIndex = 0; ListIndex < mGroupItemArray->Num(); ++ListIndex) {
													if ((*mGroupItemArray)[ListIndex] > GroupItemRow->GetUniqueId()) {
                                                        mGroupItemArray->Insert(GroupItemRow->GetUniqueId(), ListIndex);
														insertSuccess = true;
														break;
													}
												}
												if (!insertSuccess) {
                                                    mGroupItemArray->Add(GroupItemRow->GetUniqueId());
												}
											}

											{
												bool insertSuccess = false;
												for (int ListIndex = 0; ListIndex < mSelectListSource.Num(); ++ListIndex) {
                                                    FConfigTableRowBase* SelectListRow = (FConfigTableRowBase*)(mSelectListSource[ListIndex]->ConfigTableRow);
													if (SelectListRow->GetUniqueId() > GroupItemRow->GetUniqueId()) {
														mSelectListSource.Insert(GroupItemSource[Index], ListIndex);
														insertSuccess = true;
														break;
													}
												}
												if (!insertSuccess) {
													mSelectListSource.Add(GroupItemSource[Index]);
												}
											}
															
											mUnselectListSource.Remove(GroupItemSource[Index]);


											mDataTable->MarkPackageDirty();
											SelectList->RebuildList();
											unSelectList->RebuildList();
											break;
										}
									}
								}
								mUnselectListSelectItem.Reset();
							}

							return FReply::Handled();
						})
						.Content()
						[
							SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.Text(FText::FromString(">>"))
						]
					]
					+ SVerticalBox::Slot()
					[
						SNew(SButton)
						.OnClicked_Lambda([this, GroupItemSource, SelectList, unSelectList]() {
							if (mSelectListSelectItem.IsValid()) {
                                FConfigTableRowBase* SelectListSelectRow = (FConfigTableRowBase*)(mSelectListSelectItem->ConfigTableRow);
								if (mGroupItemArray->Contains(SelectListSelectRow->GetUniqueId())) {
									for (int Index = 0; Index < GroupItemSource.Num(); ++Index) {
                                        FConfigTableRowBase* GroupItemRow = (FConfigTableRowBase*)(GroupItemSource[Index]->ConfigTableRow);
										if (GroupItemRow->GetUniqueId() == SelectListSelectRow->GetUniqueId()) {
                                            mGroupItemArray->Remove(GroupItemRow->GetUniqueId());

											mSelectListSource.Remove(GroupItemSource[Index]);

											{
												bool insertSuccess = false;
												for (int ListIndex = 0; ListIndex < mUnselectListSource.Num(); ++ListIndex) {
                                                    FConfigTableRowBase* UnselectListRow = (FConfigTableRowBase*)(mUnselectListSource[ListIndex]->ConfigTableRow);
													if (UnselectListRow->GetUniqueId() > GroupItemRow->GetUniqueId()) {
														mUnselectListSource.Insert(GroupItemSource[Index], ListIndex);
														insertSuccess = true;
														break;
													}
												}
												if (!insertSuccess) {
													mUnselectListSource.Add(GroupItemSource[Index]);
												}
											}

											mDataTable->MarkPackageDirty();
											SelectList->RebuildList();
											unSelectList->RebuildList();
											break;
										}
									}
								}

								mSelectListSelectItem.Reset();
							}

							return FReply::Handled();
						})
						.Content()
						[
							SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.Text(FText::FromString("<<"))
						]
					]
					+ SVerticalBox::Slot()
					[
						SNew(SButton)
						.OnClicked_Lambda([this, GroupItemSource, SelectList, unSelectList]() {
							mGroupItemArray->Empty();

							mUnselectListSelectItem.Reset();
							mSelectListSelectItem.Reset();
							mUnselectListSource.Empty();
							mSelectListSource.Empty();

							for (int Index = 0; Index < GroupItemSource.Num(); ++Index) {
								mUnselectListSource.Add(GroupItemSource[Index]);
							}

							SelectList->RebuildList();
							unSelectList->RebuildList();

							mDataTable->MarkPackageDirty();

							return FReply::Handled();
						})
						.Content()
						[
							SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.Text(FText::FromString(TEXT("全不选")))
						]
					]
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.4)
				[
					SelectList->AsShared()
				]
			]
		];
	}
};

void SSimpleGroupBase::Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList) {
	ConstructGroupWidgetSources();

	ChildSlot
		[
			ConstructGroupPage()
		];
}

void SSimpleGroupBase::ConstructGroupWidgetSources() {
    mDataTable = GetGroupDataTable();
    mGroupSource = GetGroupSource();
}

TSharedRef<SWidget> SSimpleGroupBase::ConstructGroupPage() {
	return SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SAssignNew(mGroupListView, SListView<TSharedPtr<FConfigTableRowWrapper>>)
				.ListItemsSource(&mGroupSource)
				.SelectionMode(ESelectionMode::Single)
				.OnGenerateRow(this, &SSimpleGroupBase::GroupListViewOnGenerateRow)
				.OnSelectionChanged(this, &SSimpleGroupBase::OnGroupSelectionChanged)
				.HeaderRow
				(
					SNew(SHeaderRow)
					+ SHeaderRow::Column(GroupUI::GroupIdColumnName)
					.DefaultLabel(FText::FromName(GroupUI::GroupIdColumnName))
					.FillWidth(33.0f)
					+ SHeaderRow::Column(GroupUI::GroupDescriptionColumnName)
					.DefaultLabel(FText::FromName(GroupUI::GroupDescriptionColumnName))
					.FillWidth(53.0f)
					+ SHeaderRow::Column(GroupUI::GroupEditColumnName)
					.DefaultLabel(FText::FromName(GroupUI::GroupEditColumnName))
					.FillWidth(33.0f)
				)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				ConstructGroupRowEditor()
			]
		];
}

TSharedRef<SWidget> SSimpleGroupBase::ConstructGroupRowEditor() {
    auto GroupDataTable = GetGroupDataTable();
	mGroupEditorRow = SNew(SGameDataTableRowEditor, GroupDataTable);
	mGroupEditorRow->RowAddedCallback.BindSP(this, &SSimpleGroupBase::GroupTableRowAdded);
	mGroupEditorRow->RowPreRemoveCallback.BindSP(this, &SSimpleGroupBase::GroupTableRowPreRemove);
	mGroupEditorRow->RowRemovedCallback.BindSP(this, &SSimpleGroupBase::GroupTableRowRemoved);
	mGroupEditorRow->RowModifiedCallback.BindSP(this, &SSimpleGroupBase::GroupTableRowModified);
	mGroupEditorRow->RowSelectedCallback.BindSP(this, &SSimpleGroupBase::GroupTableSetHighlightedRow);


	return mGroupEditorRow.ToSharedRef();
}

FName SSimpleGroupBase::GroupTableRowAdded(FName Name) {
    auto GroupDataTable = GetGroupDataTable();
	if (GroupDataTable != nullptr) {
        FName NewRowName = NewRowInit(Name);
        if (!NewRowName.ToString().IsEmpty()) {
            FConfigTableRowBase* FindRow = GroupDataTable->FindRow<FConfigTableRowBase>(NewRowName, "");

            auto TableUsingStruct = GroupDataTable->GetRowStruct();
            int32 StructureSize = TableUsingStruct->GetStructureSize();
            FMoneyTypeConfigTableRow* NewRawRowData = (FMoneyTypeConfigTableRow*)FMemory::Malloc(StructureSize);
            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, FindRow);
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = (uint8*)NewRawRowData;

            mGroupSource.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));

            mGroupListView->RebuildList();
            mGroupListView->ScrollToBottom();
            return NewRowName;
        }
		else {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("创建新组错误")), 5.0);
			return FName();
		}
	}
	return Name;
}

bool SSimpleGroupBase::GroupTableRowPreRemove(FName Name) {
	return true;
}

void SSimpleGroupBase::GroupTableRowRemoved(FName Name) {
	for (auto Index = 0; Index < mGroupSource.Num(); ++Index) {
        FConfigTableRowBase* RowData = (FConfigTableRowBase*)(mGroupSource[Index]->ConfigTableRow);
		FName RowName = *FString::FromInt(RowData->GetUniqueId());
		if (RowName == Name) {			
            mGroupSource.RemoveAt(Index);
			mGroupListView->RebuildList();
			break;
		}
	}
}

void SSimpleGroupBase::GroupTableRowModified(FName Name) {
    auto GroupDataTable = GetGroupDataTable();
	if (GroupDataTable != nullptr) {
		FConfigTableRowBase* FindRow = (FConfigTableRowBase*)GroupDataTable->FindRowUnchecked(Name);
		for (auto Index = 0; Index < mGroupSource.Num(); ++Index) {
            FConfigTableRowBase* RowData = (FConfigTableRowBase*)(mGroupSource[Index]->ConfigTableRow);
			if (FindRow->GetUniqueId() == RowData->GetUniqueId()) {
                auto TableUsingStruct = GroupDataTable->GetRowStruct();
                TableUsingStruct->CopyScriptStruct(RowData, FindRow);

				mGroupListView->RebuildList();
				break;
			}
		}
	}
}

void SSimpleGroupBase::GroupTableSetHighlightedRow(FName Name) {
	if (Name == mGroupHighlightedRowName)
	{
		return;
	}

	if (Name.IsNone())
	{
		mGroupHighlightedRowName = NAME_None;

		// Synchronize the list views
		mGroupListView->ClearSelection();
	}
	else
	{
		TSharedPtr<FConfigTableRowWrapper>* NewSelectionPtr = mGroupSource.FindByPredicate([&Name](const TSharedPtr<FConfigTableRowWrapper>& RowDataWrapper) -> bool
		{
            FConfigTableRowBase* RowData = (FConfigTableRowBase*)(RowDataWrapper->ConfigTableRow);
			return *FString::FromInt(RowData->GetUniqueId()) == Name;
		});
		
		// Synchronize the list views
		if (NewSelectionPtr)
		{
			mGroupHighlightedRowName = Name;
			mGroupListView->SetSelection(*NewSelectionPtr);
		}
		else
		{
			mGroupListView->ClearSelection();
		}
	}
}

void SSimpleGroupBase::OnGroupSelectionChanged(TSharedPtr<FConfigTableRowWrapper> InNewSelection, ESelectInfo::Type InSelectInfo) {
	if (InNewSelection.IsValid()) {
        FConfigTableRowBase* RowData = (FConfigTableRowBase*)(InNewSelection->ConfigTableRow);
		FName RowName = *FString::FromInt(RowData->GetUniqueId());
		const bool bSelectionChanged = !InNewSelection.IsValid() || RowName != mGroupHighlightedRowName;
		const FName NewRowName = (InNewSelection.IsValid()) ? RowName : NAME_None;

		GroupTableSetHighlightedRow(NewRowName);

		if (bSelectionChanged)
		{
			mGroupEditorRow->SelectRow(mGroupHighlightedRowName);
		}
	}
	else {
		mGroupListView->ClearSelection();
	}
}

TSharedRef<ITableRow> SSimpleGroupBase::GroupListViewOnGenerateRow(TSharedPtr<FConfigTableRowWrapper> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SGroupRow, OwnerTable, Item, this, &mGroupSource, mDataTable);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION