#include "SConditionWidgetDefault.h"
#include "Modules/Condition/CoreCondition.h"
#include "PropertyEditorModule.h"

void SConditionWidgetDefault::Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot) {
	SConditionWidget::Construct(SConditionWidget::FArguments(), InWidgetCondition);

	ParentSlot = InParentSlot;

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
				.Text(InWidgetCondition->GetClass()->GetDisplayNameText())
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
				.OnClicked(this, &SConditionWidgetDefault::RemoveClick)
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
	TSharedPtr<class IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(WidgetCondition);
	DetailsView->OnFinishedChangingProperties().AddSP(this, &SConditionWidgetDefault::OnPropertyChanged);

	(*WidgetSlot)[
		DetailsView.ToSharedRef()
	];
}

FReply SConditionWidgetDefault::RemoveClick() {
	SConditionWidget::OnConditionWidgetPreremove.ExecuteIfBound(WidgetCondition);

	ParentSlot->DetachWidget();

	return FReply::Handled();
}

void SConditionWidgetDefault::NotifyConditionChange() {
	SConditionWidget::OnConditionWidgetChange.ExecuteIfBound(WidgetCondition);
}

void SConditionWidgetDefault::OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent) {
	NotifyConditionChange();
}