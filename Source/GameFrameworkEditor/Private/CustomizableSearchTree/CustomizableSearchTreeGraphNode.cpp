// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CustomizableSearchTreeGraphNode.h"
#include "Modules/CustomizableSearchTree/CustomizableSearchTreeNodeBase.h"

UCustomizableSearchTreeGraphNode::UCustomizableSearchTreeGraphNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

FText UCustomizableSearchTreeGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    UCustomizableSearchTreeNodeBase* CustomizableSearchTreeNode = Cast<UCustomizableSearchTreeNodeBase>(NodeInstance);

    FString ReturnNodeTitle;

    if (CustomizableSearchTreeNode->PreNode) {
        for (int Index = 0; Index < CustomizableSearchTreeNode->PreNode->FollowNodes.Num(); ++Index) {
            if (CustomizableSearchTreeNode->PreNode->FollowNodes[Index] == CustomizableSearchTreeNode) {
                ReturnNodeTitle.AppendInt(Index);
                ReturnNodeTitle.Append(TEXT("."));
                break;
            }
        }
    }

    ReturnNodeTitle.Append(CustomizableSearchTreeNode->GetNodeTitle());

    return FText::FromString(ReturnNodeTitle);
}

void UCustomizableSearchTreeGraphNode::AllocateDefaultPins() {
    UCustomizableSearchTreeNodeBase* CustomizableSearchTreeNode = Cast<UCustomizableSearchTreeNodeBase>(NodeInstance);
    CreatePin(EGPD_Input, PinCategory_MultipleNodes, TEXT("Parent"));
    auto OutputInfo = CustomizableSearchTreeNode->GetOutputPinInfo();
    for (int Index = 0; Index < OutputInfo.Num(); ++Index) {
        if (OutputInfo[Index].AllowMulti) {
            CreatePin(EGPD_Output, PinCategory_MultipleNodes, *OutputInfo[Index].Name);
        }
        else {
            CreatePin(EGPD_Output, PinCategory_SingleNode, *OutputInfo[Index].Name);
        }
    }
}

void UCustomizableSearchTreeGraphNode::RefreshNodeOnDataChanged() {
    UCustomizableSearchTreeNodeBase* CustomizableSearchTreeNode = Cast<UCustomizableSearchTreeNodeBase>(NodeInstance);
    if (CustomizableSearchTreeNode == nullptr) return;
    //更新节点数据
    UEdGraphPin* findInputPin = nullptr;
    TArray<UEdGraphPin*> outputPins;
    for (auto pin : Pins) {
        if (pin->Direction == EGPD_Input) {
            findInputPin = pin;
        }
        else {
            outputPins.Add(pin);
        }
    }
    auto OutputInfo = CustomizableSearchTreeNode->GetOutputPinInfo();
    for (int Index = 0; Index < OutputInfo.Num(); ++Index) {
        if (!outputPins.IsValidIndex(Index)) {
            if (OutputInfo[Index].AllowMulti) {
                CreatePin(EGPD_Output, PinCategory_MultipleNodes, *OutputInfo[Index].Name);
            }
            else {
                CreatePin(EGPD_Output, PinCategory_SingleNode, *OutputInfo[Index].Name);
            }
        }
        else {
            if (OutputInfo[Index].AllowMulti) {
                if (outputPins[Index]->PinType.PinCategory != PinCategory_MultipleNodes) {
                    outputPins[Index]->PinType.PinCategory = PinCategory_MultipleNodes;
                }
            }
            else {
                if (outputPins[Index]->PinType.PinCategory != PinCategory_SingleNode) {
                    outputPins[Index]->PinType.PinCategory = PinCategory_SingleNode;
                }
            }
            if (outputPins[Index]->PinName.ToString() != OutputInfo[Index].Name) {
                outputPins[Index]->PinName = *OutputInfo[Index].Name;
            }
        }
    }
    while (outputPins.Num() > OutputInfo.Num()) {
        auto removePin = outputPins[outputPins.Num() - 1];
        outputPins.RemoveAt(outputPins.Num() - 1);
        RemovePin(removePin);
    }
}