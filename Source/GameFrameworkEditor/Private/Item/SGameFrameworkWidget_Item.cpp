// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_Item.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SEditableLabel.h"
#include "GameFrameworkEditorCommands.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"

#include "Modules/Assets/BackpackSetting.h"
#include "Modules/Item/ItemSetting.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "Modules/Assets/BackpackTypeConfigTableRow.h"
#include "Modules/Item/ItemTypeConfigTableRow.h"
#include "Modules/Item/ItemConfigTableRow.h"

#include "SGameDataTableRowEditor.h"
#include "Widgets/Text/STextBlock.h"

#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "Modules/Item/ItemType.h"
#include "SRowTableRefBox.h"
#include "Modules/Skill/SkillGroupConfigTableRow.h"
#include "Modules/Skill/SkillSetting.h"

namespace BackpackTypeUI
{
	const FName BackpackTypeIdColumnName(TEXT("背包类型Id"));
	const FName BackpackInitMaxNumColumnName(TEXT("背包初始大小"));
	const FName BackpackExpansionMaxNumColumnName(TEXT("背包最大扩展大小"));
};

class SPackageTypeRow : public SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>
{
public:
	SLATE_BEGIN_ARGS(SPackageTypeRow) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& Args, const TSharedRef<STableViewBase>& OwnerTableView, TSharedPtr<FConfigTableRowWrapper> InPtr)
	{
		BackpackTypePtr = InPtr;

		SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>::Construct(
			FSuperRowType::FArguments()
			.Padding(1.0f),
			OwnerTableView
		);
	}

	/** Overridden from SMultiColumnTableRow.  Generates a widget for this column of the list view. */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
	{
        FBackpackTypeConfigTableRow* RowData = (FBackpackTypeConfigTableRow*)(BackpackTypePtr->ConfigTableRow);
		if (ColumnName == BackpackTypeUI::BackpackTypeIdColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(RowData->BackpackTypeId)));
		}
		else if (ColumnName == BackpackTypeUI::BackpackInitMaxNumColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(RowData->BackpackInitMaxNum)));
		}
		else if (ColumnName == BackpackTypeUI::BackpackExpansionMaxNumColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(RowData->BackpackExpansionMaxNum)));
		}

		return SNullWidget::NullWidget;
	}

private:

	TSharedPtr<FConfigTableRowWrapper> BackpackTypePtr;
};

namespace ItemTypeUI
{
	const FName ItemTypeIdColumnName(TEXT("物品类型Id"));
	const FName ItemTypeDescriptionColumnName(TEXT("物品类型描述"));
	const FName DefaultBackpackTypeColumnName(TEXT("默认背包类型"));
};

class SItemTypeRow : public SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>
{
public:
	SLATE_BEGIN_ARGS(SItemTypeRow) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& Args, const TSharedRef<STableViewBase>& OwnerTableView, TSharedPtr<FConfigTableRowWrapper> InPtr, TSharedPtr<SGameDataTableRowEditor> InDataTableRowEditor)
	{
		ItemTypePtr = InPtr;

		this->DataTableRowEditor = InDataTableRowEditor;

		SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>::Construct(
			FSuperRowType::FArguments()
			.Padding(1.0f),
			OwnerTableView
		);
	}

	/** Overridden from SMultiColumnTableRow.  Generates a widget for this column of the list view. */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
	{
        FItemTypeConfigTableRow* RowData = (FItemTypeConfigTableRow*)(ItemTypePtr->ConfigTableRow);
		if (ColumnName == ItemTypeUI::ItemTypeIdColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(RowData->ItemTypeId)));
		}
		else if (ColumnName == ItemTypeUI::ItemTypeDescriptionColumnName)
		{
			return	SNew(SEditableText)
				.Text(FText::FromString(RowData->ItemTypeDescription))
				.IsReadOnly(true);
		}

		return SNullWidget::NullWidget;
	}

private:

	TSharedPtr<FConfigTableRowWrapper> ItemTypePtr;

	TSharedPtr<SGameDataTableRowEditor> DataTableRowEditor;
};

namespace ItemInfoUI
{
	const FName ItemIdColumnName(TEXT("物品Id"));
	const FName ItemNameColumnName(TEXT("物品名字"));
	const FName ItemTypeColumnName(TEXT("物品类型"));
	const FName ItemDescriptionColumnName(TEXT("物品描述"));
	const FName ItemSkillGroupColumnName(TEXT("技能模组"));
};

class SItemInfoRow : public SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>
{
public:
	SLATE_BEGIN_ARGS(SItemInfoRow) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& Args, const TSharedRef<STableViewBase>& OwnerTableView, TSharedPtr<FConfigTableRowWrapper> InPtr, TSharedPtr<SGameDataTableRowEditor> InDataTableRowEditor)
	{
		ItemInfoPtr = InPtr;
        FItemConfigTableRow* RowData = (FItemConfigTableRow*)(ItemInfoPtr->ConfigTableRow);
        const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
        auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
        ItemTypeTableRefBox = SNew(SRowTableRefBox, ItemTypeDataTable, RowData->ItemType);
        ItemTypeTableRefBox->RowSelectChanged.BindSP(this, &SItemInfoRow::OnSelectionChanged);

		this->DataTableRowEditor = InDataTableRowEditor;

		SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>::Construct(
			FSuperRowType::FArguments()
			.Padding(1.0f),
			OwnerTableView
		);
	}

	/** Overridden from SMultiColumnTableRow.  Generates a widget for this column of the list view. */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
	{
        FItemConfigTableRow* RowData = (FItemConfigTableRow*)(ItemInfoPtr->ConfigTableRow);
		if (ColumnName == ItemInfoUI::ItemIdColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(RowData->ItemId)));
		}
		else if (ColumnName == ItemInfoUI::ItemNameColumnName)
		{
			return	SNew(SEditableText)
				.Text(FText::FromString(RowData->ItemName))
				.IsReadOnly(true);
		}
		else if (ColumnName == ItemInfoUI::ItemTypeColumnName)
		{
			return ItemTypeTableRefBox.ToSharedRef();
		}
		else if (ColumnName == ItemInfoUI::ItemDescriptionColumnName)
		{
			return	SNew(SEditableText)
				.Text(FText::FromString(RowData->ItemDescription))
				.IsReadOnly(true);
		}
		else if (ColumnName == ItemInfoUI::ItemSkillGroupColumnName) {
			const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
			auto SkillGroupDataTable = SkillSetting->SkillGroupTable.LoadSynchronous();
			if (!SkillGroupDataTable) {
				return SNullWidget::NullWidget;
			}
			else {
				TSharedPtr<SRowTableRefBox> SkillGroupRefBox = SNew(SRowTableRefBox, SkillGroupDataTable, RowData->SkillGroupID);
				SkillGroupRefBox->RowSelectChanged.BindLambda([this](int ID) {
					((FItemConfigTableRow*)DataTableRowEditor->GetCurrentRow()->GetStructMemory())->SkillGroupID = ID;

					DataTableRowEditor->MarkDatatableDirty();
				});
				return SkillGroupRefBox.ToSharedRef();
			}
		}

		return SNullWidget::NullWidget;
	}

private:
	TSharedPtr<FConfigTableRowWrapper> ItemInfoPtr;
    TSharedPtr<SRowTableRefBox> ItemTypeTableRefBox;

	TSharedPtr<SGameDataTableRowEditor> DataTableRowEditor;

    void OnSelectionChanged(int ItemTypeId) {
        FItemConfigTableRow* RowData = (FItemConfigTableRow*)(ItemInfoPtr->ConfigTableRow);
        DataTableRowEditor->SelectRow(*FString::FromInt(RowData->ItemId));
        ((FItemConfigTableRow*)DataTableRowEditor->GetCurrentRow()->GetStructMemory())->ItemType = ItemTypeId;

        DataTableRowEditor->MarkDatatableDirty();
    }
};

void SGameFrameworkWidget_Item::Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList) {
	ConstructItemWidgetSources();

	ChildSlot
		[
			ConstructItemPage()
		];
}

SGameFrameworkWidget_Item::~SGameFrameworkWidget_Item() {

}

void SGameFrameworkWidget_Item::ConstructItemWidgetSources() {
	mItemTabIndex = 0;

	FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();

    if (ItemTypeDataTable->GetRowNames().Num() == 0) {
        //添加默认物品类型
        auto TableUsingStruct = ItemTypeDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        const UEnum* const BuiltInItemType = StaticEnum<EBuiltInItemType>();
        for (int Index = 0; Index < BuiltInItemType->NumEnums() - 2; ++Index) {
            //增加默认的角色升级经验类型
            FItemTypeConfigTableRow* NewRawRowData = (FItemTypeConfigTableRow*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);

            NewRawRowData->ItemTypeId = BuiltInItemType->GetIndexByValue(Index);
            NewRawRowData->ItemTypeDescription = BuiltInItemType->GetDisplayNameTextByIndex(Index).ToString();

            ItemTypeDataTable->AddRow(*FString::FromInt(NewRawRowData->ItemTypeId), *NewRawRowData);
        }
    }

	mItemTypeSource = GameFrameworkEditorModule.GetEditorWidgetTool()->GetItemTypeSource();

	mItemInfoSource = GameFrameworkEditorModule.GetEditorWidgetTool()->GetItemInfoSource();
}

TSharedRef<SWidget> SGameFrameworkWidget_Item::ConstructItemPage() {
	return SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.FillWidth(0.1)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.MaxHeight(50)
				[
					ConstructItemPageButton(TEXT("背包类型"), FOnClicked::CreateRaw(this, &SGameFrameworkWidget_Item::ItemPackageTabClicked))
				]
				+ SVerticalBox::Slot()
				.MaxHeight(50)
				[
					ConstructItemPageButton(TEXT("物品类型"), FOnClicked::CreateRaw(this, &SGameFrameworkWidget_Item::ItemItemTypeTabClicked))
				]
				+ SVerticalBox::Slot()
				.MaxHeight(50)
				[
					ConstructItemPageButton(TEXT("物品"), FOnClicked::CreateRaw(this, &SGameFrameworkWidget_Item::ItemItemInfoTabClicked))
				]
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.FillWidth(0.9)
			[
				SNew(SWidgetSwitcher)
				.WidgetIndex(this, &SGameFrameworkWidget_Item::GetItemTabIndex)
				//BackpackType编辑器选项卡界面
				+ SWidgetSwitcher::Slot()
				[
					SNullWidget::NullWidget
				]
				//itemType编辑器选项卡界面
				+ SWidgetSwitcher::Slot()
				[
					ConstructItemTypePage()
				]
				//itemInfo编辑器选项卡界面
				+ SWidgetSwitcher::Slot()
				[
					ConstructItemInfoPage()
				]
			]
		];
}

int32 SGameFrameworkWidget_Item::GetItemTabIndex() const {
	return mItemTabIndex;
}

TSharedRef<SWidget> SGameFrameworkWidget_Item::ConstructItemPageButton(const FString& PageName, FOnClicked ClickCallback) {
	return SNew(SButton)
	.ContentPadding(-3)
	.OnClicked(ClickCallback)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
				.ColorAndOpacity(FLinearColor(0, 0, 0, 1))
				.Text(FText::FromString(PageName))
			]
		]
	];
}

FReply SGameFrameworkWidget_Item::ItemPackageTabClicked() {
	mItemTabIndex = 0;
	
	const UBackpackSetting* BackpackSetting = GetDefault<UBackpackSetting>();
	auto BackpackTypeTable = BackpackSetting->BackpackTypeTable.LoadSynchronous();
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(BackpackTypeTable);

	return FReply::Handled();
}

FReply SGameFrameworkWidget_Item::ItemItemTypeTabClicked() {
	mItemTabIndex = 1;
	return FReply::Handled();
}

FReply SGameFrameworkWidget_Item::ItemItemInfoTabClicked() {
	mItemTabIndex = 2;
	mItemInfoListView->RebuildList();
	return FReply::Handled();
}

TSharedRef<SWidget> SGameFrameworkWidget_Item::ConstructItemTypePage() {
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SAssignNew(mItemTypeListView, SListView<TSharedPtr<FConfigTableRowWrapper>>)
			.ListItemsSource(&mItemTypeSource)
			.SelectionMode(ESelectionMode::Single)
			.OnGenerateRow(this, &SGameFrameworkWidget_Item::ItemTypeListViewOnGenerateRow)
			.OnSelectionChanged(this, &SGameFrameworkWidget_Item::OnItemTypeSelectionChanged)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column(ItemTypeUI::ItemTypeIdColumnName)
				.DefaultLabel(FText::FromName(ItemTypeUI::ItemTypeIdColumnName))
				.FillWidth(33.0f)
				+ SHeaderRow::Column(ItemTypeUI::ItemTypeDescriptionColumnName)
				.DefaultLabel(FText::FromName(ItemTypeUI::ItemTypeDescriptionColumnName))
				.FillWidth(53.0f)
				+ SHeaderRow::Column(ItemTypeUI::DefaultBackpackTypeColumnName)
				.DefaultLabel(FText::FromName(ItemTypeUI::DefaultBackpackTypeColumnName))
				.FillWidth(53.0f)
			)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			ConstructItemTypeRowEditor()
		];
}

TSharedRef<SWidget> SGameFrameworkWidget_Item::ConstructItemTypeRowEditor() {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
	mItemTypeEditorRow = SNew(SGameDataTableRowEditor, ItemTypeDataTable);
	mItemTypeEditorRow->RowPreAddCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemTypeTableRowPreAdd);
	mItemTypeEditorRow->RowAddedCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemTypeTableRowAdded);
	mItemTypeEditorRow->RowPreRemoveCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemTypeTableRowPreRemove);
	mItemTypeEditorRow->RowRemovedCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemTypeTableRowRemoved);
	mItemTypeEditorRow->RowModifiedCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemTypeTableRowModified);
	mItemTypeEditorRow->RowSelectedCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemTypeTableSetHighlightedRow);


	return mItemTypeEditorRow.ToSharedRef();
}

bool SGameFrameworkWidget_Item::ItemTypeTableRowPreAdd(FName Name) {
	return true;
}

FName SGameFrameworkWidget_Item::ItemTypeTableRowAdded(FName Name) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
	if (ItemTypeDataTable != nullptr) {
		int32 NewItemTypeId = ApplyItemTypeId();
		if (NewItemTypeId != -1) {
			FName RowName = *FString::FromInt(NewItemTypeId);
			if (FDataTableEditorUtils::RenameRow(ItemTypeDataTable, Name, RowName)) {
				FItemTypeConfigTableRow* FindRow = ItemTypeDataTable->FindRow<FItemTypeConfigTableRow>(RowName, "");
				FindRow->ItemTypeId = NewItemTypeId;
				FindRow->ItemTypeDescription = TEXT("这里添加类型描述");
				
                auto TableUsingStruct = ItemTypeDataTable->GetRowStruct();
                int32 StructureSize = TableUsingStruct->GetStructureSize();
                FBackpackTypeConfigTableRow* NewRawRowData = (FBackpackTypeConfigTableRow*)FMemory::Malloc(StructureSize);
                TableUsingStruct->InitializeStruct(NewRawRowData);
                TableUsingStruct->CopyScriptStruct(NewRawRowData, FindRow);
                FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
                NewWrapper->RowStruct = TableUsingStruct;
                NewWrapper->ConfigTableRow = (uint8*)NewRawRowData;

                mItemTypeSource.Add(MakeShareable(NewWrapper));

				mItemTypeListView->RebuildList();
                mItemTypeListView->ScrollToBottom();
				return RowName;
			}
			else {
				return Name;
			}
		}
		else {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("物品类型不能超过255个")), 5.0);
			return FName();
		}
	}
	return Name;
}

bool SGameFrameworkWidget_Item::ItemTypeTableRowPreRemove(FName Name) {
	FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

	int itemType = FCString::Atoi(*Name.ToString());
	if (itemType <= (int32)EBuiltInItemType::E_BuiltInMax)
	{
		GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("内建物品类型不能删除")), 3.0);
		return false;
	}

	for (int32 Index = 0; Index < mItemInfoSource.Num(); ++Index) {
        FItemConfigTableRow* RowData = (FItemConfigTableRow*)(mItemInfoSource[Index]->ConfigTableRow);
		if (RowData->ItemType == itemType) {
			GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("该物品类型有正在使用的物品，不能删除")), 3.0);
			return false;
		}
	}
	return true;
}

void SGameFrameworkWidget_Item::ItemTypeTableRowRemoved(FName Name) {
	for (auto Index = 0; Index < mItemTypeSource.Num(); ++Index) {
        FItemTypeConfigTableRow* RowData = (FItemTypeConfigTableRow*)(mItemTypeSource[Index]->ConfigTableRow);
		FName RowName = *FString::FromInt(RowData->ItemTypeId);
		if (RowName == Name) {
			mItemTypeSource.RemoveAt(Index);
			mItemTypeListView->RebuildList();
			break;
		}
	}
}

void SGameFrameworkWidget_Item::ItemTypeTableRowModified(FName Name) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
	if (ItemTypeDataTable != nullptr) {
		FItemTypeConfigTableRow* FindRow = ItemTypeDataTable->FindRow<FItemTypeConfigTableRow>(Name, "");
		for (auto Index = 0; Index < mItemTypeSource.Num(); ++Index) {
            FItemTypeConfigTableRow* RowData = (FItemTypeConfigTableRow*)(mItemTypeSource[Index]->ConfigTableRow);
			if (FindRow->ItemTypeId == RowData->ItemTypeId) {
                auto TableUsingStruct = ItemTypeDataTable->GetRowStruct();
                TableUsingStruct->CopyScriptStruct(RowData, FindRow);

				mItemTypeListView->RebuildList();
				break;
			}
		}
	}
}

void SGameFrameworkWidget_Item::ItemTypeTableSetHighlightedRow(FName Name) {
	if (Name == mItemTypeHighlightedRowName)
	{
		return;
	}

	if (Name.IsNone())
	{
		mItemTypeHighlightedRowName = NAME_None;

		// Synchronize the list views
		mItemTypeListView->ClearSelection();
	}
	else
	{
		TSharedPtr<FConfigTableRowWrapper>* NewSelectionPtr = mItemTypeSource.FindByPredicate([&Name](const TSharedPtr<FConfigTableRowWrapper>& RowDataWrapper) -> bool
		{
            FItemTypeConfigTableRow* RowData = (FItemTypeConfigTableRow*)(RowDataWrapper->ConfigTableRow);
			return *FString::FromInt(RowData->ItemTypeId) == Name;
		});
		
		// Synchronize the list views
		if (NewSelectionPtr)
		{
			mItemTypeHighlightedRowName = Name;
			mItemTypeListView->SetSelection(*NewSelectionPtr);
		}
		else
		{
			mItemTypeListView->ClearSelection();
		}
	}
}

void SGameFrameworkWidget_Item::OnItemTypeSelectionChanged(TSharedPtr<FConfigTableRowWrapper> InNewSelection, ESelectInfo::Type InSelectInfo) {
	if (InNewSelection.IsValid()) {
        FItemTypeConfigTableRow* RowData = (FItemTypeConfigTableRow*)(InNewSelection->ConfigTableRow);
		FName RowName = *FString::FromInt(RowData->ItemTypeId);
		const bool bSelectionChanged = !InNewSelection.IsValid() || RowName != mItemTypeHighlightedRowName;
		const FName NewRowName = (InNewSelection.IsValid()) ? RowName : NAME_None;

		ItemTypeTableSetHighlightedRow(NewRowName);

		if (bSelectionChanged)
		{
			mItemTypeEditorRow->SelectRow(mItemTypeHighlightedRowName);
		}
	}
	else {
		mItemTypeListView->ClearSelection();
	}
}

TSharedRef<ITableRow> SGameFrameworkWidget_Item::ItemTypeListViewOnGenerateRow(TSharedPtr<FConfigTableRowWrapper> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SItemTypeRow, OwnerTable, Item, mItemTypeEditorRow);
}

int32 SGameFrameworkWidget_Item::ApplyItemTypeId() {
	auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
	auto itemTypeIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("ItemTypeIdSeed", "");
	if (itemTypeIdSeed == nullptr) {
		FDataTableSeedConfigTableRow NewRow;
		NewRow.SeedKey = "ItemTypeIdSeed";
		NewRow.SeedValue = 0;
		SeedTable->AddRow("ItemTypeIdSeed", NewRow);
		itemTypeIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("ItemTypeIdSeed", "");
	}
	TSet<int> ItemTypeIdSet;
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
	if (ItemTypeDataTable != nullptr) {
		TArray<FItemTypeConfigTableRow*> ItemTypeArr;
        ItemTypeDataTable->GetAllRows("", ItemTypeArr);
		for (auto Index = 0; Index < ItemTypeArr.Num(); ++Index) {
			ItemTypeIdSet.Add(ItemTypeArr[Index]->ItemTypeId);
		}
	}
	int ItemTypeId = -1;
	if (ItemTypeIdSet.Num() > 255 - (int)EBuiltInItemType::E_BuiltInMax) {

	}
	else {
		do {
			ItemTypeId = (++itemTypeIdSeed->SeedValue) % 255;
			if (ItemTypeId <= (int)EBuiltInItemType::E_BuiltInMax)
			{
				ItemTypeId = (int)EBuiltInItemType::E_BuiltInMax + 1;
			}
		} while (ItemTypeIdSet.Contains(ItemTypeId));
	}

	SeedTable->MarkPackageDirty();
	return ItemTypeId;
}

TSharedRef<SWidget> SGameFrameworkWidget_Item::ConstructItemInfoPage() {
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SAssignNew(mItemInfoListView, SListView<TSharedPtr<FConfigTableRowWrapper>>)
			.ListItemsSource(&mItemInfoSource)
			.SelectionMode(ESelectionMode::Single)
			.OnSelectionChanged(this, &SGameFrameworkWidget_Item::OnItemInfoSelectionChanged)
			.OnGenerateRow(this, &SGameFrameworkWidget_Item::ItemInfoListViewOnGenerateRow)
			.HeaderRow
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column(ItemInfoUI::ItemIdColumnName)
				.DefaultLabel(FText::FromName(ItemInfoUI::ItemIdColumnName))
				.FillWidth(33.0f)
				+ SHeaderRow::Column(ItemInfoUI::ItemNameColumnName)
				.DefaultLabel(FText::FromName(ItemInfoUI::ItemNameColumnName))
				.FillWidth(43.0f)
				+ SHeaderRow::Column(ItemInfoUI::ItemTypeColumnName)
				.DefaultLabel(FText::FromName(ItemInfoUI::ItemTypeColumnName))
				.FillWidth(43.0f)
				+ SHeaderRow::Column(ItemInfoUI::ItemDescriptionColumnName)
				.DefaultLabel(FText::FromName(ItemInfoUI::ItemDescriptionColumnName))
				.FillWidth(53.0f)
				+ SHeaderRow::Column(ItemInfoUI::ItemSkillGroupColumnName)
				.DefaultLabel(FText::FromName(ItemInfoUI::ItemSkillGroupColumnName))
				.FillWidth(53.0f)
			)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.AutoHeight()
		[
			ConstructItemInfoRowEditor()
		]
	;
}

TSharedRef<SWidget> SGameFrameworkWidget_Item::ConstructItemInfoRowEditor() {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
	mItemInfoEditorRow = SNew(SGameDataTableRowEditor, ItemDataTable);
	mItemInfoEditorRow->RowPreAddCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemInfoTableRowPreAdd);
	mItemInfoEditorRow->RowAddedCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemInfoTableRowAdded);
	mItemInfoEditorRow->RowPreRemoveCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemInfoTableRowPreRemove);
	mItemInfoEditorRow->RowRemovedCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemInfoTableRowRemoved);
	mItemInfoEditorRow->RowModifiedCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemInfoTableRowModified);
	mItemInfoEditorRow->RowSelectedCallback.BindSP(this, &SGameFrameworkWidget_Item::ItemInfoTableSetHighlightedRow);

	return mItemInfoEditorRow.ToSharedRef();
}

bool SGameFrameworkWidget_Item::ItemInfoTableRowPreAdd(FName Name) {
	if (mItemTypeSource.Num() > 0) {
		return true;
	}
	else {
		FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
		GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("请先增加item类型")), 3.0);
		return false;
	}
}

FName SGameFrameworkWidget_Item::ItemInfoTableRowAdded(FName Name) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
	if (ItemDataTable != nullptr) {
		int32 newItemId = ApplyItemId();
		FName RowName = *FString::FromInt(newItemId);
		if (FDataTableEditorUtils::RenameRow(ItemDataTable, Name, RowName)) {
			FItemConfigTableRow* FindRow = ItemDataTable->FindRow<FItemConfigTableRow>(RowName, "");
			FindRow->ItemId = newItemId;
			FindRow->ItemName = TEXT("这里添加物品名字");
            FItemTypeConfigTableRow* ItemTypeRow = (FItemTypeConfigTableRow*)(mItemTypeSource[0]->ConfigTableRow);
			FindRow->ItemType = ItemTypeRow->ItemTypeId;
			FindRow->ItemDescription = TEXT("这里添加物品描述");

            auto TableUsingStruct = ItemDataTable->GetRowStruct();
            int32 StructureSize = TableUsingStruct->GetStructureSize();
            FBackpackTypeConfigTableRow* NewRawRowData = (FBackpackTypeConfigTableRow*)FMemory::Malloc(StructureSize);
            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, FindRow);
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = (uint8*)NewRawRowData;

            mItemInfoSource.Add(MakeShareable(NewWrapper));

			mItemInfoListView->RebuildList();
            mItemInfoListView->ScrollToBottom();
			return RowName;
		}
		else {
			return Name;
		}
	}
	return Name;
}

bool SGameFrameworkWidget_Item::ItemInfoTableRowPreRemove(FName Name) {
	FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
	FString rowNameStr = Name.ToString();
	if (rowNameStr.IsNumeric()) {
		int32 ItemId = FCString::Atoi(*rowNameStr);
		FString UseInfo;
		if (GameFrameworkEditorModule.GetEditorWidgetTool()->IsItemIdUse(ItemId, UseInfo)) {
			GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(UseInfo), 5.0);
			return false;
		}
	}
	return true;
}

void SGameFrameworkWidget_Item::ItemInfoTableRowRemoved(FName Name) {
	for (auto Index = 0; Index < mItemInfoSource.Num(); ++Index) {
        FItemConfigTableRow* RowData = (FItemConfigTableRow*)(mItemInfoSource[Index]->ConfigTableRow);
		FName RowName = *FString::FromInt(RowData->ItemId);
		if (RowName == Name) {
			mItemInfoSource.RemoveAt(Index);
			mItemInfoListView->RebuildList();
			break;
		}
	}
}

void SGameFrameworkWidget_Item::ItemInfoTableRowModified(FName Name) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
	if (ItemDataTable != nullptr) {
		FItemConfigTableRow* FindRow = ItemDataTable->FindRow<FItemConfigTableRow>(Name, "");

		for (auto Index = 0; Index < mItemInfoSource.Num(); ++Index) {
            FItemConfigTableRow* RowData = (FItemConfigTableRow*)(mItemInfoSource[Index]->ConfigTableRow);
			if (FindRow->ItemId == RowData->ItemId) {
                auto TableUsingStruct = ItemDataTable->GetRowStruct();
                TableUsingStruct->CopyScriptStruct(RowData, FindRow);

				mItemInfoListView->RebuildList();
				break;
			}
		}
	}
}

void SGameFrameworkWidget_Item::ItemInfoTableSetHighlightedRow(FName Name) {
	if (Name == mItemInfoHighlightedRowName)
	{
		return;
	}

	if (Name.IsNone())
	{
		mItemInfoHighlightedRowName = NAME_None;

		// Synchronize the list views
		mItemInfoListView->ClearSelection();
	}
	else
	{
		TSharedPtr<FConfigTableRowWrapper>* NewSelectionPtr = mItemInfoSource.FindByPredicate([&Name](const TSharedPtr<FConfigTableRowWrapper>& RowDataWrapper) -> bool
		{
            FItemConfigTableRow* RowData = (FItemConfigTableRow*)(RowDataWrapper->ConfigTableRow);
			return *FString::FromInt(RowData->ItemId) == Name;
		});

		// Synchronize the list views
		if (NewSelectionPtr)
		{
			mItemInfoHighlightedRowName = Name;
			mItemInfoListView->SetSelection(*NewSelectionPtr);
		}
		else
		{
			mItemInfoListView->ClearSelection();
		}
	}
}

void SGameFrameworkWidget_Item::OnItemInfoSelectionChanged(TSharedPtr<FConfigTableRowWrapper> InNewSelection, ESelectInfo::Type InSelectInfo) {
	if (InNewSelection.IsValid()) {
        FItemConfigTableRow* RowData = (FItemConfigTableRow*)(InNewSelection->ConfigTableRow);
		FName RowName = *FString::FromInt(RowData->ItemId);
		const bool bSelectionChanged = !InNewSelection.IsValid() || RowName != mItemInfoHighlightedRowName;
		const FName NewRowName = (InNewSelection.IsValid()) ? RowName : NAME_None;

		ItemInfoTableSetHighlightedRow(NewRowName);

		if (bSelectionChanged)
		{
			mItemInfoEditorRow->SelectRow(mItemInfoHighlightedRowName);
		}
	}
	else {
		mItemInfoListView->ClearSelection();
	}
}

TSharedRef<ITableRow> SGameFrameworkWidget_Item::ItemInfoListViewOnGenerateRow(TSharedPtr<FConfigTableRowWrapper> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SItemInfoRow, OwnerTable, Item, mItemInfoEditorRow);
}

int32 SGameFrameworkWidget_Item::ApplyItemId() {
	auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
	auto ItemIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("ItemIdSeed", "");
	if (ItemIdSeed == nullptr) {
		FDataTableSeedConfigTableRow NewRow;
		NewRow.SeedKey = "ItemIdSeed";
		NewRow.SeedValue = 0;
		SeedTable->AddRow("ItemIdSeed", NewRow);
		ItemIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("ItemIdSeed", "");
	}
	int ItemId = ++ItemIdSeed->SeedValue;
	SeedTable->MarkPackageDirty();
	return ItemId;
}