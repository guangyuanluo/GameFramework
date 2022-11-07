// Fill out your copyright notice in the Description page of Project Settings.

#include "SRewardEditWidget.h"
#include "SlateOptMacros.h"
#include "RewardWidgetManager.h"
#include "RewardWidgetFactory.h"
#include "GameFrameworkEditorCommands.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "SRewardWidget.h"
#include "Data/Reward/CoreReward.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRewardEditWidget::Construct(const FArguments& InArgs, UObject* InOuter) {
	Outer = InOuter;
	OnRewardChange = InArgs._OnRewardChange;

	auto RewardWidgetFactories = RewardWidgetManager::GetAllFactories();
	for (int Index = 0; Index < RewardWidgetFactories.Num(); ++Index) {
		RewardNameSource.Add(MakeShareable(new FString(RewardWidgetFactories[Index]->GetRewardWidgetName())));
	}

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&RewardNameSource)
				.OnGenerateWidget(this, &SRewardEditWidget::GenerateRewardTypeComboItem)
				.OnSelectionChanged(this, &SRewardEditWidget::RewardNameComboBox_OnSelectionChanged)
				[
					SNew(STextBlock)
					.Text(this, &SRewardEditWidget::GetRewardTypeComboText)
				]
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
				.ContentPadding(-3)
				.OnClicked(this, &SRewardEditWidget::AddRewardButtonClicked)
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
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
							.ColorAndOpacity(FLinearColor(0, 0, 0, 1.0f))
							.Text(FText::FromString(TEXT("添加奖励")))
						]
					]
				]
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(RewardPage, SVerticalBox)
		]
	];
}

void SRewardEditWidget::RefreshRewardPtr(TArray<class UCoreReward*>* InRewardPtr) {
	RewardPage->ClearChildren();

	RewardPtr = InRewardPtr;

	GenerateRewardWidget();
}

void SRewardEditWidget::GenerateRewardWidget() {
	if (RewardPtr) {
		for (const auto& Reward : *RewardPtr) {
			auto& RewardSlot = RewardPage->AddSlot();
			auto Factory = RewardWidgetManager::GetFactoryByWidgetName(Reward->GetClass());
			if (Factory) {
				auto RewardWidget = Factory->CreateRewardWidget(Outer, Reward, RewardSlot);
				RewardSlot
					.AutoHeight()
					[
						RewardWidget.ToSharedRef()
					];
				RewardWidget->OnRewardWidgetChange.BindSP(this, &SRewardEditWidget::OnRewardWidgetChange);
				RewardWidget->OnRewardWidgetPreremove.BindSP(this, &SRewardEditWidget::OnRewardWidgetRemove);
			}
		}
	}
}

TSharedRef<SWidget> SRewardEditWidget::GenerateRewardTypeComboItem(TSharedPtr<FString> InItem) {
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

void SRewardEditWidget::RewardNameComboBox_OnSelectionChanged(TSharedPtr<FString> NewGroupingMode, ESelectInfo::Type SelectInfo) {
	if (!NewGroupingMode.IsValid()) {
		return;
	}
	SelectRewardName = NewGroupingMode;
}

FReply SRewardEditWidget::AddRewardButtonClicked() {
	if (SelectRewardName.IsValid()) {

		auto& RewardSlot = RewardPage->AddSlot().VAlign(VAlign_Center);
		TSharedPtr<SRewardWidget> RewardWidget = RewardWidgetManager::GetFactoryByWidgetName(*SelectRewardName)->CreateRewardWidget(Outer, nullptr, RewardSlot);

		if (RewardWidget.IsValid()) {
			RewardSlot
				.AutoHeight()
				[
					RewardWidget.ToSharedRef()
				];

			RewardWidget->OnRewardWidgetChange.BindSP(this, &SRewardEditWidget::OnRewardWidgetChange);
			RewardWidget->OnRewardWidgetPreremove.BindSP(this, &SRewardEditWidget::OnRewardWidgetRemove);

			RewardPtr->Add(RewardWidget->GetWidgetReward());

			OnRewardChange.ExecuteIfBound();

			return FReply::Handled();
		}
		else {
			return FReply::Unhandled();
		}
	}
	else {
		FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
		GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("请先选择奖励类型")), 5.0);
		return FReply::Unhandled();
	}
}

FText SRewardEditWidget::GetRewardTypeComboText() const {
	if (SelectRewardName.IsValid()) {
		return FText::FromString(*SelectRewardName);
	}
	else {
		return FText::GetEmpty();
	}
}

void SRewardEditWidget::OnRewardWidgetChange(class UCoreReward* CoreReward) {
	OnRewardChange.ExecuteIfBound();
}

void SRewardEditWidget::OnRewardWidgetRemove(class UCoreReward* CoreReward) {
	RewardPtr->Remove(CoreReward);

	OnRewardChange.ExecuteIfBound();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION