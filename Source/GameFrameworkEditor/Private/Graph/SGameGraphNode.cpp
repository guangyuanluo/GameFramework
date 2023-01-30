// Copyright Epic Games, Inc. All Rights Reserved.


#include "SGameGraphNode.h"
#include "SlateCore.h"
#include "ScenarioGraphNode.h"
#include "Text/SInlineEditableTextBlock.h"

#define LOCTEXT_NAMESPACE "GraphEditor"

// SGameGraphNode

void SGameGraphNode::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	this->GraphNode = InNode;
	this->SetCursor(EMouseCursor::CardinalCross);
	this->UpdateGraphNode();
}

void SGameGraphNode::GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const
{
	FLinearColor CurrentStateColor(1.f, 0.5f, 0.25f);
	Popups.Emplace(nullptr, CurrentStateColor, GraphNode->GetNodeTitle(ENodeTitleType::ListView).ToString());
}

FSlateColor SGameGraphNode::GetBorderBackgroundColor() const
{
	FLinearColor InactiveStateColor(0.08f, 0.08f, 0.08f);
	FLinearColor ActiveStateColorDim(0.4f, 0.3f, 0.15f);
	FLinearColor ActiveStateColorBright(1.f, 0.6f, 0.35f);
	return InactiveStateColor;
}

void SGameGraphNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();
	
	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	const FSlateBrush* NodeTypeIcon = GetNameIcon();

	FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);
	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind( this, &SGraphNode::GetContentScale );
	this->GetOrAddSlot( ENodeZone::Center )
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
#if ENGINE_MAJOR_VERSION > 4
			.BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
#else
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
#endif
			.Padding(0)
			.BorderBackgroundColor( this, &SGameGraphNode::GetBorderBackgroundColor )
			[
				SNew(SHorizontalBox)

				// PIN AREA
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(LeftNodeBox, SVerticalBox)
				]

				// STATE NAME AREA
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(10.0f)
				[
					SNew(SBorder)
#if ENGINE_MAJOR_VERSION > 4
					.BorderImage(FAppStyle::GetBrush("Graph.StateNode.ColorSpill"))
#else
					.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
#endif
					.BorderBackgroundColor( TitleShadowColor )
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Visibility(EVisibility::SelfHitTestInvisible)
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							// POPUP ERROR MESSAGE
							SAssignNew(ErrorText, SErrorText )
							.BackgroundColor( this, &SGameGraphNode::GetErrorColor )
							.ToolTipText( this, &SGameGraphNode::GetErrorMsgToolTip )
						]
						+SHorizontalBox::Slot()
                        .AutoWidth()
						.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
						[
							SNew(SVerticalBox)
							+SVerticalBox::Slot()
							.AutoHeight()
							[
								SAssignNew(InlineEditableText, SInlineEditableTextBlock)
#if ENGINE_MAJOR_VERSION > 4
								.Style(FAppStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
#else
								.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
#endif
								.Text( NodeTitle.Get(), &SNodeTitle::GetHeadTitle )
								.IsReadOnly( true )
								.IsSelected(this, &SGameGraphNode::IsSelectedExclusively)
							]
							+SVerticalBox::Slot()
							.AutoHeight()
							[
								NodeTitle.ToSharedRef()
							]
						]
					]
				]

                + SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]
			]
		];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

TSharedPtr<SToolTip> SGameGraphNode::GetComplexTooltip()
{
	return SNew(SToolTip)
		[
			SNew(SVerticalBox)	
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 5.0f, 0.0f, 0.0f))
		];
}

FText SGameGraphNode::GetPreviewCornerText() const
{
	return FText::Format(NSLOCTEXT("SGraphNode", "PreviewCornerStateText", "{0} state"), GraphNode->GetNodeTitle(ENodeTitleType::ListView));
}

const FSlateBrush* SGameGraphNode::GetNameIcon() const
{
#if ENGINE_MAJOR_VERSION > 4
	return FAppStyle::GetBrush(TEXT("Graph.StateNode.Icon"));
#else
	return FEditorStyle::GetBrush(TEXT("Graph.StateNode.Icon"));
#endif
}

#undef LOCTEXT_NAMESPACE
