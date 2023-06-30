#include "STriggerActionWidget.h"
#include "Modules/TriggerAction/CoreTriggerAction.h"
#include "PropertyEditorModule.h"

void STriggerActionWidget::Construct(const FArguments& InArgs, UCoreTriggerAction* InWidgetAction, SVerticalBox::FSlot* InParentSlot) {
	ParentSlot = InParentSlot;
	WidgetAction = InWidgetAction;

	TSharedRef<SHorizontalBox> Panel = SNew(SHorizontalBox);

	ChildSlot
	[
		SNew(SExpandableArea)
		.HeaderPadding(FMargin(2.0, 2.0))
		.Padding(FMargin(10.0f, 2.0f))
		.BorderBackgroundColor(FLinearColor(.6, .6, .6, 1.0f))
		.BodyBorderBackgroundColor(FLinearColor(1.0, 0.0, 0.0))
		.HeaderContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Fill)
			[
				SNew(STextBlock)
				.Text(InWidgetAction->GetClass()->GetDisplayNameText())
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2)
			[
				SNew(SButton)
				.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
				.ForegroundColor(FSlateColor::UseForeground())
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &STriggerActionWidget::RemoveClick)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("-"))
				]
				[
					SNew(SImage)
					.Image(FAppStyle::Get().GetBrush("Cross"))
				]
			]
		]
		.BodyContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Fill)
			[
				Panel
			]
		]
	];

	SHorizontalBox::FScopedWidgetSlotArguments AddWidgetSlot = Panel->AddSlot();
	AddWidgetSlot.AutoWidth();
	AddWidgetSlot.HAlign(HAlign_Fill);
	WidgetSlot = AddWidgetSlot.GetSlot();

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;

	TSharedPtr<class IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->OnFinishedChangingProperties().AddSP(this, &STriggerActionWidget::OnPropertyChanged);
	DetailsView->SetObject(WidgetAction);

	(*WidgetSlot)[
		DetailsView.ToSharedRef()
	];
}

FReply STriggerActionWidget::RemoveClick() {
	STriggerActionWidget::OnActionWidgetPreremove.ExecuteIfBound(WidgetAction);

	return FReply::Handled();
}

void STriggerActionWidget::NotifyActionChange() {
	STriggerActionWidget::OnActionWidgetChange.ExecuteIfBound(WidgetAction);
}

void STriggerActionWidget::OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent) {
	NotifyActionChange();
}