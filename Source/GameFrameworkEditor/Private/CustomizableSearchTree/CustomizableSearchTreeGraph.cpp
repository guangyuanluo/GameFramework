// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CustomizableSearchTreeGraph.h"
#include "CustomizableSearchTreeGraphSchema.h"
#include "CustomizableSearchTreeEditor.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraphPin.h"
#include "Modules/CustomizableSearchTree/CustomizableSearchTreeNodeBase.h"
#include "Modules/CustomizableSearchTree/CustomizableSearchTree.h"
#include "CustomizableSearchTreeGraphNode_Root.h"
#include "CustomizableSearchTreeGraphNode.h"

UCustomizableSearchTreeGraph::UCustomizableSearchTreeGraph(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Schema = UCustomizableSearchTreeGraphSchema::StaticClass();
}

void UCustomizableSearchTreeGraph::NotifyGraphPinConnectionChanged()
{
	Super::NotifyGraphPinConnectionChanged();

	UGameFrameworkGraphNode* RootNode = nullptr;
	for (UEdGraphNode* Node : Nodes)
	{
		UGameFrameworkGraphNode* GraphNode = Cast<UGameFrameworkGraphNode>(Node);
		if (GraphNode != nullptr) {
			if (RootNode == nullptr) {
				RootNode = Cast<UCustomizableSearchTreeGraphNode_Root>(GraphNode);
				if (RootNode == nullptr) {
					UCustomizableSearchTreeNodeBase* CustomizableSearchTreeNode = Cast<UCustomizableSearchTreeNodeBase>(GraphNode->NodeInstance);
					if (CustomizableSearchTreeNode != nullptr) {
						CustomizableSearchTreeNode->PreNode = nullptr;
						CustomizableSearchTreeNode->FollowNodes.Empty();
					}
				}
			}
			else {
				UCustomizableSearchTreeNodeBase* CustomizableSearchTreeNode = Cast<UCustomizableSearchTreeNodeBase>(GraphNode->NodeInstance);
				if (CustomizableSearchTreeNode != nullptr) {
					CustomizableSearchTreeNode->PreNode = nullptr;
					CustomizableSearchTreeNode->FollowNodes.Empty();
				}
			}
		}
	}


	if (RootNode->Pins[0]->LinkedTo.Num() > 0)
	{
		UGameFrameworkGraphNode* ChildNode = Cast<UGameFrameworkGraphNode>(RootNode->Pins[0]->LinkedTo[0]->GetOwningNode());
		UCustomizableSearchTreeNodeBase* CustomizableSearchTreeNode = Cast<UCustomizableSearchTreeNodeBase>(ChildNode->NodeInstance);

		auto CustomizableSearchTree = Cast<UCustomizableSearchTree>(Editor->GetObjectToEdit());
		CustomizableSearchTree->RootScenario = CustomizableSearchTreeNode;

		UpdateTreeNodeRecusive(ChildNode);
	}
}

void UCustomizableSearchTreeGraph::UpdateTreeNodeRecusive(UGameFrameworkGraphNode* Node)
{
	UCustomizableSearchTreeNodeBase* CustomizableSearchTreeNode = Cast<UCustomizableSearchTreeNodeBase>(Node->NodeInstance);
	CustomizableSearchTreeNode->Asset = Cast<UCustomizableSearchTree>(Editor->GetObjectToEdit());
	for (int PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex) {
		auto Pin = Node->Pins[PinIndex];
		if (Pin->Direction == EGPD_Output) {
			auto OutputPin = Pin;
			if (Pin->LinkedTo.Num() != 0) {
				for (int32 LinkToIndex = 0; LinkToIndex < OutputPin->LinkedTo.Num(); LinkToIndex++) {
					UGameFrameworkGraphNode* ChildNode = Cast<UGameFrameworkGraphNode>(OutputPin->LinkedTo[LinkToIndex]->GetOwningNode());
					UCustomizableSearchTreeNodeBase* CustomizableSearchTreeChildNode = Cast<UCustomizableSearchTreeNodeBase>(ChildNode->NodeInstance);
					CustomizableSearchTreeNode->FollowNodes.Add(CustomizableSearchTreeChildNode);
					CustomizableSearchTreeChildNode->PreNode = CustomizableSearchTreeNode;

					UpdateTreeNodeRecusive(ChildNode);
				}
			}
		}
	}
}