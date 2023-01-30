// Fill out your copyright notice in the Description page of Project Settings.

#include "SScenarionActionListWidget.h"
#include "SlateOptMacros.h"
#include "Graph/GameFrameworkGraphTypes.h"
#include "Modules/Scenario/ScenarioAction.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SScenarionActionListWidget::~SScenarionActionListWidget() {

}

void SScenarionActionListWidget::Construct(const FArguments& InArgs, TSharedPtr<IPropertyHandle> InActionsProperty) {
	ActionsProperty = InActionsProperty;

	TSharedPtr<struct FGraphNodeClassHelper> ClassCache = MakeShareable(new FGraphNodeClassHelper(UScenarioAction::StaticClass()));
	ClassCache->UpdateAvailableBlueprintClasses();

	FCategorizedGraphActionListBuilder ScenarioBuilder(TEXT("ScenarioAction"));

	TArray<FGameFrameworkGraphNodeClassData> NodeClasses;
	ClassCache->GatherClasses(UScenarioAction::StaticClass(), NodeClasses);

	for (auto NodeClass : NodeClasses) {
		auto ActionClass = NodeClass.GetClass();
		if (ActionClass->HasAnyClassFlags(CLASS_Abstract)) continue;
		UScenarioAction* ScenarioAction = Cast<UScenarioAction>(ActionClass->GetDefaultObject());
		auto NodeTitle = ScenarioAction->GetNodeTitle();
		if (NodeTitle.IsEmpty()) continue;
		ActionClassMap.Add(NodeTitle, ActionClass);
		ActionClassNameSource.Add(MakeShareable(new FString(NodeTitle)));
	}

	ActionListPanel = SNew(SVerticalBox);

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
				.OptionsSource(&ActionClassNameSource)
				.OnGenerateWidget(this, &SScenarionActionListWidget::GenerateActionComboItem)
				.OnSelectionChanged(this, &SScenarionActionListWidget::ActionNameComboBox_OnSelectionChanged)
				[
					SNew(STextBlock)
					.Text(this, &SScenarionActionListWidget::GetActionTypeComboText)
				]
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
				.ContentPadding(-3)
				.ForegroundColor(FSlateColor::UseForeground())
				.OnClicked(this, &SScenarionActionListWidget::AddActionButtonClicked)
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
			ActionListPanel.ToSharedRef()
		]
	];

	RefreshActionListPanel();
}

TSharedRef<SWidget> SScenarionActionListWidget::GenerateActionComboItem(TSharedPtr<FString> InItem) {
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

void SScenarionActionListWidget::ActionNameComboBox_OnSelectionChanged(TSharedPtr<FString> NewGroupingMode, ESelectInfo::Type SelectInfo) {
	if (!NewGroupingMode.IsValid()) {
		return;
	}
	SelectActionClassName = NewGroupingMode;
}

FReply SScenarionActionListWidget::AddActionButtonClicked() {
	if (SelectActionClassName.IsValid()) {
		auto SelectClass = ActionClassMap.Find(*SelectActionClassName);
		TArray<UObject*> Outers;
		ActionsProperty->GetOuterObjects(Outers);
		auto ActionObject = NewObject<UScenarioAction>(Outers[0], *SelectClass);

		void* PropertyValuePtr;
		ActionsProperty->GetValueData(PropertyValuePtr);
		TArray<UScenarioAction*>* ActionListPtr = (TArray<UScenarioAction*>*)PropertyValuePtr;
		ActionListPtr->Add(ActionObject);

		RefreshActionListPanel();

		return FReply::Handled();
	}
	else {
		FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
		GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("请先选择动作类型")), 5.0);
		return FReply::Unhandled();
	}
}

FText SScenarionActionListWidget::GetActionTypeComboText() const {
	if (SelectActionClassName.IsValid()) {
		return FText::FromString(*SelectActionClassName);
	}
	else {
		return FText::GetEmpty();
	}
}

void SScenarionActionListWidget::RefreshActionListPanel() {
	ActionListPanel->ClearChildren();

	void* PropertyValuePtr;
	ActionsProperty->GetValueData(PropertyValuePtr);
	TArray<UScenarioAction*>* ActionListPtr = (TArray<UScenarioAction*>*)PropertyValuePtr;
	for (int Index = 0; Index < ActionListPtr->Num(); ++Index) {
		auto ScenarioAction = (*ActionListPtr)[Index];
		int CurrentIndex = Index;
		FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		FDetailsViewArgs DetailsViewArgs;
		DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
		TSharedPtr<class IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
		DetailsView->SetObject(ScenarioAction);
		DetailsView->OnFinishedChangingProperties().AddSP(this, &SScenarionActionListWidget::OnPropertyChanged);

		ActionListPanel->AddSlot()[
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
				.OnClicked(this, &SScenarionActionListWidget::RemoveClick, CurrentIndex)
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

void SScenarionActionListWidget::OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent) {
	OnMarkDirty();
}

FReply SScenarionActionListWidget::RemoveClick(int CurrentIndex) {
	void* PropertyValuePtr;
	ActionsProperty->GetValueData(PropertyValuePtr);
	TArray<UScenarioAction*>* ActionListPtr = (TArray<UScenarioAction*>*)PropertyValuePtr;
	ActionListPtr->RemoveAt(CurrentIndex);

	RefreshActionListPanel();

	OnMarkDirty();

	return FReply::Handled();
}

void SScenarionActionListWidget::OnMarkDirty() {
	TArray<UObject*> Outers;
	ActionsProperty->GetOuterObjects(Outers);
	Outers[0]->MarkPackageDirty();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION