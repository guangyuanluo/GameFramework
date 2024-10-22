// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_SkillMain.h"
#include "Modules/Skill/SkillSetting.h"

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
		];
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
				.ColorAndOpacity(FLinearColor(1, 1, 1, 1))
				.Text(FText::FromString(PageName))
			]
		]
	];
}

FReply SGameFrameworkWidget_SkillMain::SkillTabClicked() {
	const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
	auto SkillTable = SkillSetting->SkillTable.LoadSynchronous();
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(SkillTable);

	return FReply::Handled();
}

FReply SGameFrameworkWidget_SkillMain::EffectTabClicked() {
	const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
	auto EffectTable = SkillSetting->EffectTable.LoadSynchronous();
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(EffectTable);

	return FReply::Handled();
}

FReply SGameFrameworkWidget_SkillMain::SkillGroupTabClicked() {
	const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
	auto SkillGroupTable = SkillSetting->SkillGroupTable.LoadSynchronous();
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(SkillGroupTable);

	return FReply::Handled();
}