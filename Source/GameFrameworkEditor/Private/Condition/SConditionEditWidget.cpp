// Fill out your copyright notice in the Description page of Project Settings.

#include "SConditionEditWidget.h"
#include "SlateOptMacros.h"
#include "ConditionWidgetManager.h"
#include "ConditionWidgetFactory.h"
#include "GameFrameworkEditorCommands.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "SConditionWidget.h"
#include "Modules/Condition/CoreCondition.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SConditionEditWidget::Construct(const FArguments& InArgs, UObject* InOuter) {
	Outer = InOuter;
	OnConditionChange = InArgs._OnConditionChange;

	auto ConditionWidgetFactories = ConditionWidgetManager::GetAllFactories();
	for (int Index = 0; Index < ConditionWidgetFactories.Num(); ++Index) {
		ConditionNameSource.Add(MakeShareable(new FString(ConditionWidgetFactories[Index]->GetConditionWidgetName())));
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
				.OptionsSource(&ConditionNameSource)
				.OnGenerateWidget(this, &SConditionEditWidget::GenerateConditionTypeComboItem)
				.OnSelectionChanged(this, &SConditionEditWidget::ConditionNameComboBox_OnSelectionChanged)
				[
					SNew(STextBlock)
					.Text(this, &SConditionEditWidget::GetConditionTypeComboText)
				]
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
				.ContentPadding(-3)
				.ForegroundColor(FSlateColor::UseForeground())
				.OnClicked(this, &SConditionEditWidget::AddConditionButtonClicked)
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
							.Text(FText::FromString(TEXT("添加条件")))
						]
					]
				]
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(ConditionPage, SVerticalBox)
		]
	];
}

void SConditionEditWidget::RefreshConditionPtr(TArray<class UCoreCondition*>* InConditionPtr) {
	ConditionPage->ClearChildren();

	ConditionPtr = InConditionPtr;

	GenerateConditionWidget();
}

void SConditionEditWidget::GenerateConditionWidget() {
	if (ConditionPtr) {
		bool HaveInvalidData = false;
		for (int Index = ConditionPtr->Num() - 1; Index >= 0; --Index) {
			auto Condition = (*ConditionPtr)[Index];
			if (Condition) {
				auto& ConditionSlot = ConditionPage->AddSlot();
				auto Factory = ConditionWidgetManager::GetFactoryByWidgetName(Condition->GetClass());
				if (Factory) {
					auto ConditionWidget = Factory->CreateConditionWidget(Outer, Condition, ConditionSlot);
					ConditionSlot
						.AutoHeight()
						[
							ConditionWidget.ToSharedRef()
						];
					ConditionWidget->OnConditionWidgetChange.BindSP(this, &SConditionEditWidget::OnConditionWidgetChange);
					ConditionWidget->OnConditionWidgetPreremove.BindSP(this, &SConditionEditWidget::OnConditionWidgetRemove);
				}
			}
			else {
				ConditionPtr->RemoveAt(Index);
				HaveInvalidData = true;
			}
		}
		if (HaveInvalidData) {
			OnConditionChange.ExecuteIfBound();
		}
	}
}

TSharedRef<SWidget> SConditionEditWidget::GenerateConditionTypeComboItem(TSharedPtr<FString> InItem) {
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

void SConditionEditWidget::ConditionNameComboBox_OnSelectionChanged(TSharedPtr<FString> NewGroupingMode, ESelectInfo::Type SelectInfo) {
	if (!NewGroupingMode.IsValid()) {
		return;
	}
	SelectConditionName = NewGroupingMode;
}

FReply SConditionEditWidget::AddConditionButtonClicked() {
	if (SelectConditionName.IsValid()) {

		auto& ConditionSlot = ConditionPage->AddSlot().VAlign(VAlign_Center);
		TSharedPtr<SConditionWidget> ConditionWidget = ConditionWidgetManager::GetFactoryByWidgetName(*SelectConditionName)->CreateConditionWidget(Outer, nullptr, ConditionSlot);

		if (ConditionWidget.IsValid()) {
			ConditionSlot
				.AutoHeight()
				[
					ConditionWidget.ToSharedRef()
				];

			ConditionWidget->OnConditionWidgetChange.BindSP(this, &SConditionEditWidget::OnConditionWidgetChange);
			ConditionWidget->OnConditionWidgetPreremove.BindSP(this, &SConditionEditWidget::OnConditionWidgetRemove);

			ConditionPtr->Add(ConditionWidget->GetWidgetCondition());

			OnConditionChange.ExecuteIfBound();

			return FReply::Handled();
		}
		else {
			return FReply::Unhandled();
		}
	}
	else {
		FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
		GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("请先选择条件类型")), 5.0);
		return FReply::Unhandled();
	}
}

FText SConditionEditWidget::GetConditionTypeComboText() const {
	if (SelectConditionName.IsValid()) {
		return FText::FromString(*SelectConditionName);
	}
	else {
		return FText::GetEmpty();
	}
}

void SConditionEditWidget::OnConditionWidgetChange(class UCoreCondition* CoreCondition) {
	OnConditionChange.ExecuteIfBound();
}

void SConditionEditWidget::OnConditionWidgetRemove(class UCoreCondition* CoreCondition) {
	ConditionPtr->Remove(CoreCondition);

	OnConditionChange.ExecuteIfBound();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION