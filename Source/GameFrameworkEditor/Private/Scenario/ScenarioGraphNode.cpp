// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ScenarioGraphNode.h"
#include "Modules/Scenario/ScenarioNode.h"

UScenarioGraphNode::UScenarioGraphNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

FText UScenarioGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    UScenarioNode* scenarioNode = Cast<UScenarioNode>(NodeInstance);
    return FText::FromString(scenarioNode->GetNodeTitle());
}

void UScenarioGraphNode::AllocateDefaultPins() {
    UScenarioNode* ScenarioNode = Cast<UScenarioNode>(NodeInstance);
    CreatePin(EGPD_Input, PinCategory_MultipleNodes, TEXT("Parent"));
    auto OutputInfo = ScenarioNode->GetOutputPinInfo();
    for (int Index = 0; Index < OutputInfo.Num(); ++Index) {
        if (OutputInfo[Index].AllowMulti) {
            CreatePin(EGPD_Output, PinCategory_MultipleNodes, *OutputInfo[Index].Name);
        }
        else {
            CreatePin(EGPD_Output, PinCategory_SingleNode, *OutputInfo[Index].Name);
        }
    }
}

void UScenarioGraphNode::RefreshNodeOnDataChanged() {
    UScenarioNode* scenarioNode = Cast<UScenarioNode>(NodeInstance);
    if (scenarioNode == nullptr) return;
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
    auto OutputInfo = scenarioNode->GetOutputPinInfo();
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