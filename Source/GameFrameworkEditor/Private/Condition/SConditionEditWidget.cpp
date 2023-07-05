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
#include "JsonObjectConverter.h"
#include "Modules/Condition/CoreConditionList.h"

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
							.ColorAndOpacity(FLinearColor::White)
							.Text(FText::FromString(TEXT("添加条件")))
						]
					]
				]
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
				.ContentPadding(-3)
				.ForegroundColor(FSlateColor::UseForeground())
				.OnClicked(this, &SConditionEditWidget::CopyButtonClicked)
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
				.OnClicked(this, &SConditionEditWidget::PasteButtonClicked)
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
				.OnClicked(this, &SConditionEditWidget::ClearButtonClicked)
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
			if (!Condition) {
				ConditionPtr->RemoveAt(Index);
				HaveInvalidData = true;
			}
		}

		for (int Index = 0; Index < ConditionPtr->Num(); ++Index) {
			auto Condition = (*ConditionPtr)[Index];
#if ENGINE_MAJOR_VERSION > 4
			auto ConditionSlot = ConditionPage->AddSlot();
#else
			auto& ConditionSlot = ConditionPage->AddSlot();
#endif
			
			TSharedPtr<class SConditionWidget> ConditionWidget = SNew(SConditionWidgetDefault, Condition, ConditionSlot.GetSlot(), Index);
			ConditionSlot
				.AutoHeight()
				[
					ConditionWidget.ToSharedRef()
				];
			ConditionWidget->OnConditionWidgetChange.BindSP(this, &SConditionEditWidget::OnConditionWidgetChange);
			ConditionWidget->OnConditionWidgetPreremove.BindSP(this, &SConditionEditWidget::OnConditionWidgetRemove);
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
		auto FindConditionClassPtr = ConditionNameMap.Find(*SelectConditionName);
		if (!FindConditionClassPtr) {
			return FReply::Unhandled();
		}
		bool CanCreate = true;
		auto ConditionFactory = ConditionWidgetManager::GetFactoryByConditionClass(*FindConditionClassPtr);
		if (ConditionFactory) {
			CanCreate = ConditionFactory->CanCreateCondition();
		}
		if (!CanCreate) {
			return FReply::Unhandled();
		}
		auto NewCondition = NewObject<UCoreCondition>(Outer, *FindConditionClassPtr);
		if (ConditionFactory) {
			ConditionFactory->PostInitConditionCreated(NewCondition);
		}
		ConditionPtr->Add(NewCondition);

		ConditionPage->ClearChildren();
		GenerateConditionWidget();

		OnConditionChange.ExecuteIfBound();

		return FReply::Handled();
	}
	else {
		FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
		GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("请先选择条件类型")), 5.0);
		return FReply::Unhandled();
	}
}

FReply SConditionEditWidget::CopyButtonClicked() {
	TSharedRef<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject());

	FCoreConditionList ConditionList;
	ConditionList.Conditions = *ConditionPtr;

	FJsonObjectConverter::UStructToJsonObject(FCoreConditionList::StaticStruct(), &ConditionList, RootJsonObject, 0, 0);

	typedef TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FStringWriter;
	typedef TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FStringWriterFactory;
	
	FString SerializeStr;
	TSharedRef<FStringWriter> Writer = FStringWriterFactory::Create(&SerializeStr);
	FJsonSerializer::Serialize(RootJsonObject, Writer);

	FWindowsPlatformMisc::ClipboardCopy(*SerializeStr);

	return FReply::Handled();
}

FReply SConditionEditWidget::PasteButtonClicked() {
	FString SerializeStr;

	FWindowsPlatformMisc::ClipboardPaste(SerializeStr);

	FCoreConditionList ConditionList;
	if (!FJsonObjectConverter::JsonObjectStringToUStruct<FCoreConditionList>(SerializeStr, &ConditionList)) {
		return FReply::Unhandled();
	}

	for (auto Condition : ConditionList.Conditions) {
		auto NewCondition = DuplicateObject(Condition, Outer);
		ConditionPtr->Add(NewCondition);
	}

	ConditionPage->ClearChildren();
	GenerateConditionWidget();

	OnConditionChange.ExecuteIfBound();

	return FReply::Handled();
}

FReply SConditionEditWidget::ClearButtonClicked() {
	ConditionPtr->Empty();

	ConditionPage->ClearChildren();
	GenerateConditionWidget();

	OnConditionChange.ExecuteIfBound();

	return FReply::Handled();
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

	ConditionPage->ClearChildren();
	GenerateConditionWidget();

	OnConditionChange.ExecuteIfBound();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION