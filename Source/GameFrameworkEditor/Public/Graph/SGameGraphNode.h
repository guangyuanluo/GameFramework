// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class GAMEFRAMEWORKEDITOR_API SGameGraphNode : public SGraphNode {
public:
    SLATE_BEGIN_ARGS(SGameGraphNode) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

    // SNodePanel::SNode interface
    virtual void GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const override;
    // End of SNodePanel::SNode interface

    // SGraphNode interface
    virtual void UpdateGraphNode() override;
    virtual TSharedPtr<SToolTip> GetComplexTooltip() override;
    // End of SGraphNode interface
protected:
    FSlateColor GetBorderBackgroundColor() const;
    virtual FText GetPreviewCornerText() const;
    virtual const FSlateBrush* GetNameIcon() const;
};