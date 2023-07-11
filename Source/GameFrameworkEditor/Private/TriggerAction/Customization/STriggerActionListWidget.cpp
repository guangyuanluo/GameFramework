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
#include "JsonObjectConverter.h"
#include "Modules/TriggerAction/CoreTriggerActionList.h"
#include "PropertyCustomizationHelpers.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STriggerActionListWidget::Construct(const FArguments& InArgs, UObject* InOuter) {
	Outer = InOuter;
	OnActionChange = InArgs._OnActionChange;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(SClassPropertyEntryBox)
					.MetaClass(UCoreTriggerAction::StaticClass())
					.ShowDisplayNames(true)
					.ShowTreeView(true)
					.SelectedClass_Lambda([this]()
					{
						return SelectActionClass;
					})
					.OnSetClass_Lambda([this](const UClass* SelectedClass)
					{
						SelectActionClass = const_cast<UClass*>(SelectedClass);
					})
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
							.ColorAndOpacity(FLinearColor::White)
							.Text(FText::FromString(TEXT("添加触发动作")))
						]
					]
				]
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
				.ContentPadding(-3)
				.ForegroundColor(FSlateColor::UseForeground())
				.OnClicked(this, &STriggerActionListWidget::CopyButtonClicked)
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
							.ColorAndOpacity(FLinearColor::White)
							.Text(FText::FromString(TEXT("复制")))
						]
					]
				]
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
				.ContentPadding(-3)
				.ForegroundColor(FSlateColor::UseForeground())
				.OnClicked(this, &STriggerActionListWidget::PasteButtonClicked)
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
							.ColorAndOpacity(FLinearColor::White)
							.Text(FText::FromString(TEXT("粘贴")))
						]
					]
				]
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
				.ContentPadding(-3)
				.ForegroundColor(FSlateColor::UseForeground())
				.OnClicked(this, &STriggerActionListWidget::ClearButtonClicked)
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
							.ColorAndOpacity(FLinearColor::White)
							.Text(FText::FromString(TEXT("清空")))
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

FReply STriggerActionListWidget::AddActionButtonClicked() {
	if (SelectActionClass) {
		auto NewAction = NewObject<UCoreTriggerAction>(Outer, SelectActionClass);
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

FReply STriggerActionListWidget::CopyButtonClicked() {
	TSharedRef<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject());

	FCoreTriggerActionList ActionList;
	ActionList.Actions = *ActionArrayPtr;

	FJsonObjectConverter::UStructToJsonObject(FCoreTriggerActionList::StaticStruct(), &ActionList, RootJsonObject, 0, 0);

	typedef TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FStringWriter;
	typedef TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FStringWriterFactory;

	FString SerializeStr;
	TSharedRef<FStringWriter> Writer = FStringWriterFactory::Create(&SerializeStr);
	FJsonSerializer::Serialize(RootJsonObject, Writer);

	FWindowsPlatformMisc::ClipboardCopy(*SerializeStr);

	return FReply::Handled();
}

FReply STriggerActionListWidget::PasteButtonClicked() {
	FString SerializeStr;

	FWindowsPlatformMisc::ClipboardPaste(SerializeStr);

	FCoreTriggerActionList ActionList;
	if (!FJsonObjectConverter::JsonObjectStringToUStruct<FCoreTriggerActionList>(SerializeStr, &ActionList)) {
		return FReply::Unhandled();
	}

	for (auto Action : ActionList.Actions) {
		auto NewAction = DuplicateObject(Action, Outer);
		ActionArrayPtr->Add(NewAction);
	}

	TriggerActionPage->ClearChildren();
	GenerateActionWidget();

	OnActionChange.ExecuteIfBound();

	return FReply::Handled();
}

FReply STriggerActionListWidget::ClearButtonClicked() {
	ActionArrayPtr->Empty();

	TriggerActionPage->ClearChildren();
	GenerateActionWidget();

	OnActionChange.ExecuteIfBound();

	return FReply::Handled();
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