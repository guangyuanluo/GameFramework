// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SVector2InputBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "HAL/IConsoleManager.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

#define LOCTEXT_NAMESPACE "SVector2InputBox"

TAutoConsoleVariable<float> CVar2CrushThem(TEXT("Slate.AllowNumericLabelCrush"), 1.0f, TEXT("Should we crush the vector input box?."));
TAutoConsoleVariable<float> CVar2StopCrushWhenAbove(TEXT("Slate.NumericLabelWidthCrushStop"), 200.0f, TEXT("Stop crushing when the width is above."));
TAutoConsoleVariable<float> CVar2StartCrushWhenBelow(TEXT("Slate.NumericLabelWidthCrushStart"), 190.0f, TEXT("Start crushing when the width is below."));

void SVector2InputBox::Construct( const FArguments& InArgs )
{
	bCanBeCrushed = InArgs._AllowResponsiveLayout;

	TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox);

	ChildSlot
	[
		HorizontalBox
	];

	ConstructX( InArgs, HorizontalBox );
	ConstructY( InArgs, HorizontalBox );
}

void SVector2InputBox::ConstructX( const FArguments& InArgs, TSharedRef<SHorizontalBox> HorizontalBox )
{	
	const FLinearColor LabelColor = InArgs._bColorAxisLabels ?  SNumericEntryBox<float>::RedLabelBackgroundColor : FLinearColor(0.0f,0.0f,0.0f,.5f);
	TSharedRef<SWidget> LabelWidget = BuildDecoratorLabel(LabelColor, FLinearColor::White, LOCTEXT("X_Label", "X"));
	TAttribute<FMargin> MarginAttribute;
	if (bCanBeCrushed)
	{
		MarginAttribute = TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SVector2InputBox::GetTextMargin));
	}

	HorizontalBox->AddSlot()
	.VAlign( VAlign_Center )
	.FillWidth( 1.0f )
	.Padding( 0.0f, 1.0f, 2.0f, 1.0f )
	[
		SNew( SNumericEntryBox<float> )
		.AllowSpin(InArgs._AllowSpin)
		.Font( InArgs._Font )
		.Value( InArgs._X )
		.OnValueChanged( InArgs._OnXChanged )
		.OnValueCommitted( InArgs._OnXCommitted )		
		.ToolTipText( LOCTEXT("X_ToolTip", "X Value") )
		.UndeterminedString( LOCTEXT("MultipleValues", "Multiple Values") )
		.LabelPadding(0)
		.OverrideTextMargin(MarginAttribute)
		.ContextMenuExtender( InArgs._ContextMenuExtenderX )
		.TypeInterface(InArgs._TypeInterface)
		.Label()
		[
			LabelWidget
		]
	];
	
}

void SVector2InputBox::ConstructY( const FArguments& InArgs, TSharedRef<SHorizontalBox> HorizontalBox )
{
	const FLinearColor LabelColor = InArgs._bColorAxisLabels ?  SNumericEntryBox<float>::GreenLabelBackgroundColor : FLinearColor(0.0f,0.0f,0.0f,.5f);
	TSharedRef<SWidget> LabelWidget = BuildDecoratorLabel(LabelColor, FLinearColor::White, LOCTEXT("Y_Label", "Y"));
	TAttribute<FMargin> MarginAttribute;
	if (bCanBeCrushed)
	{
		MarginAttribute = TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SVector2InputBox::GetTextMargin));
	}

	HorizontalBox->AddSlot()
	.VAlign( VAlign_Center )
	.FillWidth( 1.0f )
	.Padding( 0.0f, 1.0f, 2.0f, 1.0f )
	[
		SNew( SNumericEntryBox<float> )
		.AllowSpin(InArgs._AllowSpin)
		.Font( InArgs._Font )
		.Value( InArgs._Y )
		.OnValueChanged( InArgs._OnYChanged )
		.OnValueCommitted( InArgs._OnYCommitted )
		.ToolTipText( LOCTEXT("Y_ToolTip", "Y Value") )
		.UndeterminedString( LOCTEXT("MultipleValues", "Multiple Values") )
		.LabelPadding(0)
		.OverrideTextMargin(MarginAttribute)
		.ContextMenuExtender(InArgs._ContextMenuExtenderY)
		.TypeInterface(InArgs._TypeInterface)
		.Label()
		[
			LabelWidget
		]
	];

}

TSharedRef<SWidget> SVector2InputBox::BuildDecoratorLabel(FLinearColor BackgroundColor, FLinearColor InForegroundColor, FText Label)
{
	TSharedRef<SWidget> LabelWidget = SNumericEntryBox<float>::BuildLabel(Label, InForegroundColor, BackgroundColor);

	TSharedRef<SWidget> ResultWidget = LabelWidget;
	
	if (bCanBeCrushed)
	{
		ResultWidget =
			SNew(SWidgetSwitcher)
			.WidgetIndex(this, &SVector2InputBox::GetLabelActiveSlot)
			+SWidgetSwitcher::Slot()
			[
				LabelWidget
			]
			+SWidgetSwitcher::Slot()
			[
				SNew(SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("NumericEntrySpinBox.NarrowDecorator"))
				.BorderBackgroundColor(BackgroundColor)
				.ForegroundColor(InForegroundColor)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.Padding(FMargin(5.0f, 0.0f, 0.0f, 0.0f))
			];
	}

	return ResultWidget;
}

int32 SVector2InputBox::GetLabelActiveSlot() const
{
	return bIsBeingCrushed ? 1 : 0;
}

FMargin SVector2InputBox::GetTextMargin() const
{
	return bIsBeingCrushed ? FMargin(1.0f, 2.0f) : FMargin(4.0f, 2.0f);
}

void SVector2InputBox::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	bool bFoop = bCanBeCrushed && (CVar2CrushThem.GetValueOnAnyThread() > 0.0f);

	if (bFoop)
	{
		const float AlottedWidth = AllottedGeometry.GetLocalSize().X;

		const float CrushBelow = CVar2StartCrushWhenBelow.GetValueOnAnyThread();
		const float StopCrushing = CVar2StopCrushWhenAbove.GetValueOnAnyThread();

		if (bIsBeingCrushed)
		{
			bIsBeingCrushed = AlottedWidth < StopCrushing;
		}
		else
		{
			bIsBeingCrushed = AlottedWidth < CrushBelow;
		}
	}
	else
	{
		bIsBeingCrushed = false;
	}

	SCompoundWidget::OnArrangeChildren(AllottedGeometry, ArrangedChildren);
}

#undef LOCTEXT_NAMESPACE
