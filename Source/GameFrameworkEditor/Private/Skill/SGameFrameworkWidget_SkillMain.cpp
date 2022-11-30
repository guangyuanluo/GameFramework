// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_SkillMain.h"
#include "SGameFrameworkWidget_SkillTab.h"
#include "SGameFrameworkWidget_EffectTab.h"
#include "SGameFrameworkWidget_SkillGroupTab.h"

void SGameFrameworkWidget_SkillMain::Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList) {
	CommandList = InCommandList;

	ChildSlot
		[
			ConstructPage()
		];
}

SGameFrameworkWidget_SkillMain::~SGameFrameworkWidget_SkillMain() {

}

TSharedRef<SWidget> SGameFrameworkWidget_SkillMain::ConstructPage() {
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
					ConstructPageButton(TEXT("技能"), FOnClicked::CreateRaw(this, &SGameFrameworkWidget_SkillMain::SkillTabClicked))
				]
				+ SVerticalBox::Slot()
				.MaxHeight(50)
				[
					ConstructPageButton(TEXT("效果"), FOnClicked::CreateRaw(this, &SGameFrameworkWidget_SkillMain::EffectTabClicked))
				]
				+ SVerticalBox::Slot()
				.MaxHeight(50)
				[
					ConstructPageButton(TEXT("技能模组"), FOnClicked::CreateRaw(this, &SGameFrameworkWidget_SkillMain::SkillGroupTabClicked))
				]
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.FillWidth(0.9)
			[
				SNew(SWidgetSwitcher)
				.WidgetIndex(this, &SGameFrameworkWidget_SkillMain::GetTabIndex)
				+ SWidgetSwitcher::Slot()
				[
					ConstructSkillPage()
				]
				+ SWidgetSwitcher::Slot()
				[
					ConstructEffectPage()
				]
				+ SWidgetSwitcher::Slot()
				[
					ConstructSkillGroupPage()
				]
			]
		];
}

int32 SGameFrameworkWidget_SkillMain::GetTabIndex() const {
	return mTabIndex;
}

TSharedRef<SWidget> SGameFrameworkWidget_SkillMain::ConstructPageButton(const FString& PageName, FOnClicked ClickCallback) {
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

FReply SGameFrameworkWidget_SkillMain::SkillTabClicked() {
	mTabIndex = 0;
	return FReply::Handled();
}

FReply SGameFrameworkWidget_SkillMain::EffectTabClicked() {
	mTabIndex = 1;
	return FReply::Handled();
}

FReply SGameFrameworkWidget_SkillMain::SkillGroupTabClicked() {
	mTabIndex = 2;
	return FReply::Handled();
}

TSharedRef<SWidget> SGameFrameworkWidget_SkillMain::ConstructSkillPage() {
	return SNew(SGameFrameworkWidget_SkillTab, CommandList);
}

TSharedRef<SWidget> SGameFrameworkWidget_SkillMain::ConstructEffectPage() {
	return SNew(SGameFrameworkWidget_EffectTab, CommandList);
}

TSharedRef<SWidget> SGameFrameworkWidget_SkillMain::ConstructSkillGroupPage() {
	return SNew(SGameFrameworkWidget_SkillGroupTab, CommandList);
}