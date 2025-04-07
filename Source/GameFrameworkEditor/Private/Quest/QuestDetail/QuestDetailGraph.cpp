// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "QuestDetailGraph.h"
#include "QuestDetailGraphSchema.h"
#include "QuestTreeEditor.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraphPin.h"
#include "QuestDetailGraphNode.h"
#include "QuestDetailGraphNode_Root.h"
#include "Modules/Quest/QuestDetail.h"
#include "Modules/Quest/QuestDetailNode.h"

UQuestDetailGraph::UQuestDetailGraph(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Schema = UQuestDetailGraphSchema::StaticClass();
}

void UQuestDetailGraph::NotifyGraphPinConnectionChanged()
{
	Super::NotifyGraphPinConnectionChanged();

	UGameFrameworkGraphNode* RootNode = nullptr;
	for (UEdGraphNode* Node : Nodes) {
		UGameFrameworkGraphNode* GraphNode = Cast<UGameFrameworkGraphNode>(Node);
		if (GraphNode != nullptr) {
			if (RootNode == nullptr) {
				RootNode = Cast<UQuestDetailGraphNode_Root>(GraphNode);
				
			}
			UQuestDetailNode* QuestDetailNode = Cast<UQuestDetailNode>(GraphNode->NodeInstance);
			if (QuestDetailNode != nullptr) {
				QuestDetailNode->PreNode = nullptr;
				QuestDetailNode->NextNodes.Empty();
			}
		}
	}


	if (RootNode->Pins[0]->LinkedTo.Num() > 0) {
		UGameFrameworkGraphNode* ChildNode = Cast<UGameFrameworkGraphNode>(RootNode->Pins[0]->LinkedTo[0]->GetOwningNode());
		UQuestDetailNode* QuestDetailNode = Cast<UQuestDetailNode>(ChildNode->NodeInstance);

		auto QuestDetail = Cast<UQuestDetail>(Editor->GetObjectToEdit());
		QuestDetail->Root = QuestDetailNode;

		UpdateQuestNodeRecusive(ChildNode);
	}
}

void UQuestDetailGraph::UpdateQuestNodeRecusive(UGameFrameworkGraphNode* Node)
{
	UQuestDetailNode* QuestDetailNode = Cast<UQuestDetailNode>(Node->NodeInstance);
	for (int PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex) {
		auto Pin = Node->Pins[PinIndex];
		if (Pin->Direction == EGPD_Output) {
			auto OutputPin = Pin;
			if (OutputPin != nullptr && OutputPin->LinkedTo.Num() != 0) {
				for (int32 LinkToIndex = 0; LinkToIndex < OutputPin->LinkedTo.Num(); LinkToIndex++) {
					UGameFrameworkGraphNode* ChildNode = Cast<UGameFrameworkGraphNode>(OutputPin->LinkedTo[LinkToIndex]->GetOwningNode());
					UQuestDetailNode* QuestDetailNodeChild = Cast<UQuestDetailNode>(ChildNode->NodeInstance);
					QuestDetailNode->NextNodes.Add(QuestDetailNodeChild);
					QuestDetailNodeChild->PreNode = QuestDetailNode;

					UpdateQuestNodeRecusive(ChildNode);
				}
			}
		}
	}
}