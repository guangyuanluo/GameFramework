// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_Item.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SEditableLabel.h"
#include "GameFrameworkEditorCommands.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"

#include "Modules/Assets/BackpackSetting.h"
#include "Modules/Item/ItemSetting.h"
#include "Widgets/Text/STextBlock.h"
#include "AssetToolsModule.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "SRowTableRefBox.h"

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
					SNullWidget::NullWidget
				]
				//itemInfo编辑器选项卡界面
				+ SWidgetSwitcher::Slot()
				[
					SNullWidget::NullWidget
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

	const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
	auto ItemTypeTable = ItemSetting->ItemTypeTable.LoadSynchronous();
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(ItemTypeTable);

	return FReply::Handled();
}

FReply SGameFrameworkWidget_Item::ItemItemInfoTabClicked() {
	mItemTabIndex = 2;
	
	const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
	auto ItemTable = ItemSetting->ItemTable.LoadSynchronous();
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(ItemTable);

	return FReply::Handled();
}