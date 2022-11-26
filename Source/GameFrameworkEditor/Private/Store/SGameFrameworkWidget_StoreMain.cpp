// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_StoreMain.h"
#include "SGameFrameworkWidget_GoodsTab.h"
#include "SGameFrameworkWidget_StoreTab.h"

void SGameFrameworkWidget_StoreMain::Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList) {
	CommandList = InCommandList;

	ChildSlot
		[
			ConstructPage()
		];
}

SGameFrameworkWidget_StoreMain::~SGameFrameworkWidget_StoreMain() {

}

TSharedRef<SWidget> SGameFrameworkWidget_StoreMain::ConstructPage() {
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
					ConstructPageButton(TEXT("商品"), FOnClicked::CreateRaw(this, &SGameFrameworkWidget_StoreMain::GoodsTabClicked))
				]
				+ SVerticalBox::Slot()
				.MaxHeight(50)
				[
					ConstructPageButton(TEXT("商店"), FOnClicked::CreateRaw(this, &SGameFrameworkWidget_StoreMain::StoreTabClicked))
				]
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.FillWidth(0.9)
			[
				SNew(SWidgetSwitcher)
				.WidgetIndex(this, &SGameFrameworkWidget_StoreMain::GetTabIndex)
				//BackpackType编辑器选项卡界面
				+ SWidgetSwitcher::Slot()
				[
					ConstructGoodsPage()
				]
				//itemType编辑器选项卡界面
				+ SWidgetSwitcher::Slot()
				[
					ConstructStorePage()
				]
			]
		];
}

int32 SGameFrameworkWidget_StoreMain::GetTabIndex() const {
	return mTabIndex;
}

TSharedRef<SWidget> SGameFrameworkWidget_StoreMain::ConstructPageButton(const FString& PageName, FOnClicked ClickCallback) {
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

FReply SGameFrameworkWidget_StoreMain::GoodsTabClicked() {
	mTabIndex = 0;
	return FReply::Handled();
}

FReply SGameFrameworkWidget_StoreMain::StoreTabClicked() {
	mTabIndex = 1;
	return FReply::Handled();
}

TSharedRef<SWidget> SGameFrameworkWidget_StoreMain::ConstructGoodsPage() {
	return SNew(SGameFrameworkWidget_GoodsTab, CommandList);
}

TSharedRef<SWidget> SGameFrameworkWidget_StoreMain::ConstructStorePage() {
	return SNew(SGameFrameworkWidget_StoreTab, CommandList);
}