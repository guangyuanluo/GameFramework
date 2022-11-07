// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ScenarioGraph.h"
#include "ScenarioGraphSchema.h"
#include "ScenarioEditor.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraphPin.h"
#include "Modules/Scenario/ScenarioNode.h"
#include "Modules/Scenario/Scenario.h"
#include "ScenarioGraphNode_Root.h"
#include "ScenarioGraphNode.h"

UScenarioGraph::UScenarioGraph(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Schema = UScenarioGraphSchema::StaticClass();
}

void UScenarioGraph::NotifyGraphPinConnectionChanged()
{
	Super::NotifyGraphPinConnectionChanged();

	UGameFrameworkGraphNode* rootNode = nullptr;
	for (UEdGraphNode* node : Nodes)
	{
		UGameFrameworkGraphNode* graphNode = Cast<UGameFrameworkGraphNode>(node);
		if (graphNode != nullptr) {
			if (rootNode == nullptr) {
				rootNode = Cast<UScenarioGraphNode_Root>(graphNode);
				if (rootNode == nullptr) {
					UScenarioNode* scenarioNode = Cast<UScenarioNode>(graphNode->NodeInstance);
					if (scenarioNode != nullptr) {
						scenarioNode->PreScenarioNode = nullptr;
						scenarioNode->FollowScenarioNodes.Empty();
					}
				}
			}
			else {
				UScenarioNode* scenarioNode = Cast<UScenarioNode>(graphNode->NodeInstance);
				if (scenarioNode != nullptr) {
					scenarioNode->PreScenarioNode = nullptr;
					scenarioNode->FollowScenarioNodes.Empty();
				}
			}
		}
	}


	if (rootNode->Pins[0]->LinkedTo.Num() > 0)
	{
		UGameFrameworkGraphNode* childNode = Cast<UGameFrameworkGraphNode>(rootNode->Pins[0]->LinkedTo[0]->GetOwningNode());
		UScenarioNode* scenarioNode = Cast<UScenarioNode>(childNode->NodeInstance);

		auto Scenario = Cast<UScenario>(Editor->GetObjectToEdit());
		Scenario->RootScenario = scenarioNode;

		UpdateScenarioNodeRecusive(childNode);
	}
}

void UScenarioGraph::UpdateScenarioNodeRecusive(UGameFrameworkGraphNode* node)
{
	UScenarioNode* scenarioNode = Cast<UScenarioNode>(node->NodeInstance);
	scenarioNode->ScenarioAsset = Cast<UScenario>(Editor->GetObjectToEdit());
	for (int PinIndex = 0; PinIndex < node->Pins.Num(); ++PinIndex) {
		auto Pin = node->Pins[PinIndex];
		if (Pin->Direction == EGPD_Output) {
			auto OutputPin = Pin;
			if (Pin->LinkedTo.Num() != 0) {
				for (int32 LinkToIndex = 0; LinkToIndex < OutputPin->LinkedTo.Num(); LinkToIndex++) {
					UGameFrameworkGraphNode* childNode = Cast<UGameFrameworkGraphNode>(OutputPin->LinkedTo[LinkToIndex]->GetOwningNode());
					UScenarioNode* scenarioChildNode = Cast<UScenarioNode>(childNode->NodeInstance);
					scenarioNode->FollowScenarioNodes.Add(scenarioChildNode);
					scenarioChildNode->PreScenarioNode = scenarioNode;

					UpdateScenarioNodeRecusive(childNode);
				}
			}
		}
	}
}