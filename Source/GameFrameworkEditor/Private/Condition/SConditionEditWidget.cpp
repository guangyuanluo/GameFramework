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
#include "SConditionWidgetDefault.h"
#include "Graph/GameFrameworkGraphTypes.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SConditionEditWidget::Construct(const FArguments& InArgs, UObject* InOuter) {
	Outer = InOuter;
	OnConditionChange = InArgs._OnConditionChange;

	TSharedPtr<struct FGraphNodeClassHelper> ClassCache = MakeShareable(new FGraphNodeClassHelper(UCoreCondition::StaticClass()));
	ClassCache->UpdateAvailableBlueprintClasses();

	FCategorizedGraphActionListBuilder ConditionBuilder(TEXT("Condition"));

	TArray<FGameFrameworkGraphNodeClassData> ConditionClassDatas;
	ClassCache->GatherClasses(UCoreCondition::StaticClass(), ConditionClassDatas);

	for (auto& ConditionClassData : ConditionClassDatas) {
		auto ConditionClass = ConditionClassData.GetClass();
		if (ConditionClass->HasAnyClassFlags(CLASS_Abstract)) continue;
		auto ConditionClassName = ConditionClass->GetDisplayNameText().ToString();
		ConditionNameSource.Add(MakeShareable(new FString(ConditionClassName)));
		ConditionNameMap.Add(ConditionClassName, ConditionClass);
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
#if ENGINE_MAJOR_VERSION > 4
				auto ConditionSlot = ConditionPage->AddSlot();
#else
				auto& ConditionSlot = ConditionPage->AddSlot();
#endif

				TSharedPtr<class SConditionWidget> ConditionWidget;
				auto Factory = ConditionWidgetManager::GetFactoryByWidgetName(Condition->GetClass());
				if (Factory) {
#if ENGINE_MAJOR_VERSION > 4
					ConditionWidget = Factory->CreateConditionWidget(Outer, Condition, ConditionSlot.GetSlot());
#else
					ConditionWidget = Factory->CreateConditionWidget(Outer, Condition, &ConditionSlot);
#endif
				}
				else {
					ConditionWidget = SNew(SConditionWidgetDefault, Condition, ConditionSlot.GetSlot());
				}
				ConditionSlot
					.AutoHeight()
					[
						ConditionWidget.ToSharedRef()
					];
				ConditionWidget->OnConditionWidgetChange.BindSP(this, &SConditionEditWidget::OnConditionWidgetChange);
				ConditionWidget->OnConditionWidgetPreremove.BindSP(this, &SConditionEditWidget::OnConditionWidgetRemove);
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

		TSharedPtr<class SConditionWidget> ConditionWidget;
#if ENGINE_MAJOR_VERSION > 4
		auto ConditionSlot = ConditionPage->AddSlot();
		auto ConditionFactory = ConditionWidgetManager::GetFactoryByWidgetName(*SelectConditionName);
		if (ConditionFactory) {
			ConditionWidget = ConditionFactory->CreateConditionWidget(Outer, nullptr, ConditionSlot.GetSlot());
		}
#else
		auto& ConditionSlot = ConditionPage->AddSlot();
		auto ConditionFactory = ConditionWidgetManager::GetFactoryByWidgetName(*SelectConditionName);
		if (ConditionFactory) {
			ConditionWidget = ConditionFactory->CreateConditionWidget(Outer, nullptr, &ConditionSlot);
		}
#endif
		if (!ConditionWidget.IsValid()) {
			auto FindConditionClassPtr = ConditionNameMap.Find(*SelectConditionName);
			if (FindConditionClassPtr) {
				auto Condition = NewObject<UCoreCondition>(Outer, *FindConditionClassPtr);
				ConditionWidget = SNew(SConditionWidgetDefault, Condition, ConditionSlot.GetSlot());
			}
		}

		if (ConditionWidget.IsValid()) {
			ConditionSlot
				.AutoHeight()
				.VAlign(VAlign_Center)
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