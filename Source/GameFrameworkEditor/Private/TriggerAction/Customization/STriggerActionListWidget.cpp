// Fill out your copyright notice in the Description page of Project Settings.

#include "STriggerActionListWidget.h"
#include "SlateOptMacros.h"
#include "ConditionWidgetManager.h"
#include "ConditionWidgetFactory.h"
#include "GameFrameworkEditorCommands.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "STriggerActionWidget.h"
#include "Modules/TriggerAction/CoreTriggerAction.h"
#include "Graph/GameFrameworkGraphTypes.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STriggerActionListWidget::Construct(const FArguments& InArgs, UObject* InOuter) {
	Outer = InOuter;
	OnActionChange = InArgs._OnActionChange;

	TSharedPtr<struct FGraphNodeClassHelper> ClassCache = MakeShareable(new FGraphNodeClassHelper(UCoreTriggerAction::StaticClass()));
	ClassCache->UpdateAvailableBlueprintClasses();

	FCategorizedGraphActionListBuilder ActionBuilder(TEXT("TriggerAction"));

	TArray<FGameFrameworkGraphNodeClassData> TriggerActionClassDatas;
	ClassCache->GatherClasses(UCoreTriggerAction::StaticClass(), TriggerActionClassDatas);

	for (auto& ActionClassData : TriggerActionClassDatas) {
		auto ActionClass = ActionClassData.GetClass();
		if (ActionClass->HasAnyClassFlags(CLASS_Abstract)) continue;
		auto ActionClassName = ActionClass->GetDisplayNameText().ToString();
		ActionNameSource.Add(MakeShareable(new FString(ActionClassName)));
		ActionNameMap.Add(ActionClassName, ActionClass);
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
				.OptionsSource(&ActionNameSource)
				.OnGenerateWidget(this, &STriggerActionListWidget::GenerateActionTypeComboItem)
				.OnSelectionChanged(this, &STriggerActionListWidget::ActionNameComboBox_OnSelectionChanged)
				[
					SNew(STextBlock)
					.Text(this, &STriggerActionListWidget::GetActionTypeComboText)
				]
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
				.ContentPadding(-3)
				.ForegroundColor(FSlateColor::UseForeground())
				.OnClicked(this, &STriggerActionListWidget::AddActionButtonClicked)
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
							.Text(FText::FromString(TEXT("添加触发动作")))
						]
					]
				]
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(TriggerActionPage, SVerticalBox)
		]
	];
}

void STriggerActionListWidget::RefreshActionArrayPtr(TArray<class UCoreTriggerAction*>* InActionArrayPtr) {
	TriggerActionPage->ClearChildren();

	ActionArrayPtr = InActionArrayPtr;

	GenerateActionWidget();
}

void STriggerActionListWidget::GenerateActionWidget() {
	if (ActionArrayPtr) {
		bool HaveInvalidData = false;

		for (int Index = ActionArrayPtr->Num() - 1; Index >= 0; --Index) {
			auto Action = (*ActionArrayPtr)[Index];
			if (!Action) {
				ActionArrayPtr->RemoveAt(Index);
				HaveInvalidData = true;
			}
		}

		for (int Index = 0; Index < ActionArrayPtr->Num(); ++Index) {
			auto Action = (*ActionArrayPtr)[Index];
			auto ActionSlot = TriggerActionPage->AddSlot();
			
			TSharedPtr<class STriggerActionWidget> ActionWidget = SNew(STriggerActionWidget, Action, ActionSlot.GetSlot());
			ActionSlot
				.AutoHeight()
				[
					ActionWidget.ToSharedRef()
				];
			ActionWidget->OnActionWidgetChange.BindSP(this, &STriggerActionListWidget::OnActionWidgetChange);
			ActionWidget->OnActionWidgetPreremove.BindSP(this, &STriggerActionListWidget::OnActionWidgetRemove);
		}
		if (HaveInvalidData) {
			OnActionChange.ExecuteIfBound();
		}
	}
}

TSharedRef<SWidget> STriggerActionListWidget::GenerateActionTypeComboItem(TSharedPtr<FString> InItem) {
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

void STriggerActionListWidget::ActionNameComboBox_OnSelectionChanged(TSharedPtr<FString> NewGroupingMode, ESelectInfo::Type SelectInfo) {
	if (!NewGroupingMode.IsValid()) {
		return;
	}
	SelectActionName = NewGroupingMode;
}

FReply STriggerActionListWidget::AddActionButtonClicked() {
	if (SelectActionName.IsValid()) {
		auto FindConditionClassPtr = ActionNameMap.Find(*SelectActionName);
		if (!FindConditionClassPtr) {
			return FReply::Unhandled();
		}
		auto NewAction = NewObject<UCoreTriggerAction>(Outer, *FindConditionClassPtr);
		ActionArrayPtr->Add(NewAction);

		TriggerActionPage->ClearChildren();
		GenerateActionWidget();

		OnActionChange.ExecuteIfBound();

		return FReply::Handled();
	}
	else {
		FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
		GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("请先选择触发动作类型")), 5.0);
		return FReply::Unhandled();
	}
}

FText STriggerActionListWidget::GetActionTypeComboText() const {
	if (SelectActionName.IsValid()) {
		return FText::FromString(*SelectActionName);
	}
	else {
		return FText::GetEmpty();
	}
}

void STriggerActionListWidget::OnActionWidgetChange(class UCoreTriggerAction* CoreAction) {
	OnActionChange.ExecuteIfBound();
}

void STriggerActionListWidget::OnActionWidgetRemove(class UCoreTriggerAction* CoreAction) {
	ActionArrayPtr->Remove(CoreAction);

	TriggerActionPage->ClearChildren();
	GenerateActionWidget();

	OnActionChange.ExecuteIfBound();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION