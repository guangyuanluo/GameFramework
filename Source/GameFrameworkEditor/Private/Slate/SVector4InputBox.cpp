// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SVector4InputBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "HAL/IConsoleManager.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

#define LOCTEXT_NAMESPACE "SVector4InputBox"

TAutoConsoleVariable<float> CVar4CrushThem(TEXT("Slate.AllowNumericLabelCrush"), 1.0f, TEXT("Should we crush the vector input box?."));
TAutoConsoleVariable<float> CVar4StopCrushWhenAbove(TEXT("Slate.NumericLabelWidthCrushStop"), 200.0f, TEXT("Stop crushing when the width is above."));
TAutoConsoleVariable<float> CVar4StartCrushWhenBelow(TEXT("Slate.NumericLabelWidthCrushStart"), 190.0f, TEXT("Start crushing when the width is below."));

void SVector4InputBox::Construct( const FArguments& InArgs )
{
	bCanBeCrushed = InArgs._AllowResponsiveLayout;

	TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox);

	ChildSlot
	[
		HorizontalBox
	];

	ConstructX( InArgs, HorizontalBox );
	ConstructY( InArgs, HorizontalBox );
	ConstructZ( InArgs, HorizontalBox );
	ConstructW( InArgs, HorizontalBox );
}

void SVector4InputBox::ConstructX( const FArguments& InArgs, TSharedRef<SHorizontalBox> HorizontalBox )
{	
	const FLinearColor LabelColor = InArgs._bColorAxisLabels ?  SNumericEntryBox<float>::RedLabelBackgroundColor : FLinearColor(0.0f,0.0f,0.0f,.5f);
	TSharedRef<SWidget> LabelWidget = BuildDecoratorLabel(LabelColor, FLinearColor::White, LOCTEXT("X_Label", "X"));
	TAttribute<FMargin> MarginAttribute;
	if (bCanBeCrushed)
	{
		MarginAttribute = TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SVector4InputBox::GetTextMargin));
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

void SVector4InputBox::ConstructY( const FArguments& InArgs, TSharedRef<SHorizontalBox> HorizontalBox )
{
	const FLinearColor LabelColor = InArgs._bColorAxisLabels ?  SNumericEntryBox<float>::GreenLabelBackgroundColor : FLinearColor(0.0f,0.0f,0.0f,.5f);
	TSharedRef<SWidget> LabelWidget = BuildDecoratorLabel(LabelColor, FLinearColor::White, LOCTEXT("Y_Label", "Y"));
	TAttribute<FMargin> MarginAttribute;
	if (bCanBeCrushed)
	{
		MarginAttribute = TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SVector4InputBox::GetTextMargin));
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

void SVector4InputBox::ConstructZ( const FArguments& InArgs, TSharedRef<SHorizontalBox> HorizontalBox )
{
	const FLinearColor LabelColor = InArgs._bColorAxisLabels ?  SNumericEntryBox<float>::BlueLabelBackgroundColor : FLinearColor(0.0f,0.0f,0.0f,.5f);
	TSharedRef<SWidget> LabelWidget = BuildDecoratorLabel(LabelColor, FLinearColor::White, LOCTEXT("Z_Label", "Z"));
	TAttribute<FMargin> MarginAttribute;
	if (bCanBeCrushed)
	{
		MarginAttribute = TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SVector4InputBox::GetTextMargin));
	}

	HorizontalBox->AddSlot()
	.VAlign( VAlign_Center )
	.FillWidth( 1.0f )
	.Padding( 0.0f, 1.0f, 0.0f, 1.0f )
	[
		SNew( SNumericEntryBox<float> )
		.AllowSpin(InArgs._AllowSpin)
		.Font( InArgs._Font )
		.Value( InArgs._Z )
		.OnValueChanged( InArgs._OnZChanged )
		.OnValueCommitted( InArgs._OnZCommitted )
		.ToolTipText( LOCTEXT("Z_ToolTip", "Z Value") )
		.UndeterminedString( LOCTEXT("MultipleValues", "Multiple Values") )
		.LabelPadding(0)
		.OverrideTextMargin(MarginAttribute)
		.ContextMenuExtender(InArgs._ContextMenuExtenderZ)
		.TypeInterface(InArgs._TypeInterface)
		.Label()
		[
			LabelWidget
		]
	];
}

void SVector4InputBox::ConstructW( const FArguments& InArgs, TSharedRef<SHorizontalBox> HorizontalBox )
{
	const FLinearColor LabelColor = InArgs._bColorAxisLabels ?  SNumericEntryBox<float>::BlueLabelBackgroundColor : FLinearColor(0.0f,0.0f,0.0f,.5f);
	TSharedRef<SWidget> LabelWidget = BuildDecoratorLabel(LabelColor, FLinearColor::White, LOCTEXT("W_Label", "W"));
	TAttribute<FMargin> MarginAttribute;
	if (bCanBeCrushed)
	{
		MarginAttribute = TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SVector4InputBox::GetTextMargin));
	}

	HorizontalBox->AddSlot()
	.VAlign( VAlign_Center )
	.FillWidth( 1.0f )
	.Padding( 0.0f, 1.0f, 0.0f, 1.0f )
	[
		SNew( SNumericEntryBox<float> )
		.AllowSpin(InArgs._AllowSpin)
		.Font( InArgs._Font )
		.Value( InArgs._W )
		.OnValueChanged( InArgs._OnWChanged )
		.OnValueCommitted( InArgs._OnWCommitted )
		.ToolTipText( LOCTEXT("W_ToolTip", "W Value") )
		.UndeterminedString( LOCTEXT("MultipleValues", "Multiple Values") )
		.LabelPadding(0)
		.OverrideTextMargin(MarginAttribute)
		.ContextMenuExtender(InArgs._ContextMenuExtenderW)
		.TypeInterface(InArgs._TypeInterface)
		.Label()
		[
			LabelWidget
		]
	];
}

TSharedRef<SWidget> SVector4InputBox::BuildDecoratorLabel(FLinearColor BackgroundColor, FLinearColor InForegroundColor, FText Label)
{
	TSharedRef<SWidget> LabelWidget = SNumericEntryBox<float>::BuildLabel(Label, InForegroundColor, BackgroundColor);

	TSharedRef<SWidget> ResultWidget = LabelWidget;
	
	if (bCanBeCrushed)
	{
		ResultWidget =
			SNew(SWidgetSwitcher)
			.WidgetIndex(this, &SVector4InputBox::GetLabelActiveSlot)
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

int32 SVector4InputBox::GetLabelActiveSlot() const
{
	return bIsBeingCrushed ? 1 : 0;
}

FMargin SVector4InputBox::GetTextMargin() const
{
	return bIsBeingCrushed ? FMargin(1.0f, 2.0f) : FMargin(4.0f, 2.0f);
}

void SVector4InputBox::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	bool bFoop = bCanBeCrushed && (CVar4CrushThem.GetValueOnAnyThread() > 0.0f);

	if (bFoop)
	{
		const float AlottedWidth = AllottedGeometry.GetLocalSize().X;

		const float CrushBelow = CVar4StartCrushWhenBelow.GetValueOnAnyThread();
		const float StopCrushing = CVar4StopCrushWhenAbove.GetValueOnAnyThread();

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
