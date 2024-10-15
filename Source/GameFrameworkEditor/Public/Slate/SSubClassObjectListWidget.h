// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Slate.h"
#include "Widgets/SCompoundWidget.h"
#include "Graph/GameFrameworkGraphTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"

template<typename ClassType>
class SSubClassObjectListWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSubClassObjectListWidget<ClassType>) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<IPropertyHandle> InProperty) {
		Property = InProperty;

		TSharedPtr<struct FGameGraphNodeClassHelper> ClassCache = MakeShareable(new FGameGraphNodeClassHelper(ClassType::StaticClass()));
		ClassCache->UpdateAvailableBlueprintClasses();

		FCategorizedGraphActionListBuilder ClassBuilder(TEXT("ClassBuilder"));

		TArray<FGameFrameworkGraphNodeClassData> NodeClasses;
		ClassCache->GatherClasses(ClassType::StaticClass(), NodeClasses);

		for (auto NodeClass : NodeClasses) {
			auto ChildClass = NodeClass.GetClass();
			if (ChildClass->HasAnyClassFlags(CLASS_Abstract)) continue;
			ClassType* ChildDefaultObject = Cast<ClassType>(ChildClass->GetDefaultObject());
			auto NodeTitle = ChildDefaultObject->GetNodeTitle();
			if (NodeTitle.IsEmpty()) continue;
			ClassMap.Add(NodeTitle, ChildClass);
			ClassNameSource.Add(MakeShareable(new FString(NodeTitle)));
		}

		ListPanel = SNew(SVerticalBox);

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
					.OptionsSource(&ClassNameSource)
					.OnGenerateWidget(this, &SSubClassObjectListWidget<ClassType>::GenerateComboItem)
					.OnSelectionChanged(this, &SSubClassObjectListWidget<ClassType>::NameComboBox_OnSelectionChanged)
					[
						SNew(STextBlock)
						.Text(this, &SSubClassObjectListWidget<ClassType>::GetComboText)
					]
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SButton)
					.ContentPadding(-3)
					.ForegroundColor(FSlateColor::UseForeground())
					.OnClicked(this, &SSubClassObjectListWidget<ClassType>::AddButtonClicked)
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
								.Text(FText::FromString(TEXT("添加")))
							]
						]
					]
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				ListPanel.ToSharedRef()
			]
		];

		RefreshListPanel();
	}

	void RefreshPropertyHandle(TSharedPtr<IPropertyHandle> InProperty) {
		Property = InProperty;

		RefreshListPanel();
	}

protected:
	TMap<FString, UClass*> ClassMap;
	TArray<TSharedPtr<FString>> ClassNameSource;
	TSharedPtr<FString> SelectClassName;
	TSharedPtr<IPropertyHandle> Property;
	TSharedPtr<SVerticalBox> ListPanel;

	TSharedRef<SWidget> GenerateComboItem(TSharedPtr<FString> InItem) {
		return SNew(STextBlock)
			.Text(FText::FromString(*InItem));
	}

	void NameComboBox_OnSelectionChanged(TSharedPtr<FString> NewGroupingMode, ESelectInfo::Type SelectInfo) {
		if (!NewGroupingMode.IsValid()) {
			return;
		}
		SelectClassName = NewGroupingMode;
	}

	FText GetComboText() const {
		if (SelectClassName.IsValid()) {
			return FText::FromString(*SelectClassName);
		}
		else {
			return FText::GetEmpty();
		}
	}

	FReply AddButtonClicked() {
		if (SelectClassName.IsValid()) {
			auto SelectClass = ClassMap.Find(*SelectClassName);
			TArray<UObject*> Outers;
			Property->GetOuterObjects(Outers);
			auto CreateObject = NewObject<ClassType>(Outers[0], *SelectClass);

			void* PropertyValuePtr;
			Property->GetValueData(PropertyValuePtr);
			TArray<ClassType*>* ListPtr = (TArray<ClassType*>*)PropertyValuePtr;
			ListPtr->Add(CreateObject);

			RefreshListPanel();

			return FReply::Handled();
		}
		else {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("请先选择类型")), 5.0);
			return FReply::Unhandled();
		}
	}

	void RefreshListPanel() {
		ListPanel->ClearChildren();

		void* PropertyValuePtr;
		Property->GetValueData(PropertyValuePtr);
		TArray<ClassType*>* ListPtr = (TArray<ClassType*>*)PropertyValuePtr;
		for (int Index = 0; Index < ListPtr->Num(); ++Index) {
			auto ListItem = (*ListPtr)[Index];
			int CurrentIndex = Index;
			FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			FDetailsViewArgs DetailsViewArgs;
			DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
			TSharedPtr<class IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
			DetailsView->SetObject(ListItem);
			DetailsView->OnFinishedChangingProperties().AddSP(this, &SSubClassObjectListWidget<ClassType>::OnPropertyChanged);

			ListPanel->AddSlot()[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					DetailsView.ToSharedRef()
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(2)
				[
					SNew(SButton)
#if ENGINE_MAJOR_VERSION > 4
					.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
#else
					.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
#endif
					.ForegroundColor(FSlateColor::UseForeground())
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.OnClicked(this, &SSubClassObjectListWidget<ClassType>::RemoveClick, CurrentIndex)
					.Content()
					[
						SNew(STextBlock)
						.Text(FText::FromString("-"))
					]
					[
						SNew(SImage)
#if ENGINE_MAJOR_VERSION > 4
						.Image(FAppStyle::Get().GetBrush("Cross"))
#else
						.Image(FEditorStyle::Get().GetBrush("Cross"))
#endif
					]
				]
			];
		}
	}

	void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent) {
		OnMarkDirty();
	}

	FReply RemoveClick(int CurrentIndex) {
		void* PropertyValuePtr;
		Property->GetValueData(PropertyValuePtr);
		TArray<ClassType*>* ListPtr = (TArray<ClassType*>*)PropertyValuePtr;
		ListPtr->RemoveAt(CurrentIndex);

		RefreshListPanel();

		OnMarkDirty();

		return FReply::Handled();
	}

	void OnMarkDirty() {
		TArray<UObject*> Outers;
		Property->GetOuterObjects(Outers);
		Outers[0]->MarkPackageDirty();
	}
};