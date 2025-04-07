// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "QuestTreeGraph.h"
#include "QuestTreeGraphSchema.h"
#include "QuestTreeEditor.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraphPin.h"
#include "Modules/Quest/Quest.h"
#include "Modules/Quest/QuestTree.h"
#include "QuestTreeGraphNode.h"
#include "QuestTreeGraphNode_Root.h"

UQuestTreeGraph::UQuestTreeGraph(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Schema = UQuestTreeGraphSchema::StaticClass();
}

void UQuestTreeGraph::NotifyGraphPinConnectionChanged()
{
	Super::NotifyGraphPinConnectionChanged();

	UGameFrameworkGraphNode* RootNode = nullptr;
	for (UEdGraphNode* Node : Nodes) {
		UGameFrameworkGraphNode* GraphNode = Cast<UGameFrameworkGraphNode>(Node);
		if (GraphNode != nullptr) {
			if (RootNode == nullptr) {
				RootNode = Cast<UQuestTreeGraphNode_Root>(GraphNode);
				
			}
			UQuest* Quest = Cast<UQuest>(GraphNode->NodeInstance);
			if (Quest != nullptr) {
				if (!Quest->ID.IsValid()) {
					Quest->ID = FGuid::NewGuid();
					Editor->GetObjectToEdit()->MarkPackageDirty();
				}
				Quest->FollowQuests.Empty();
			}
		}
	}


	if (RootNode->Pins[0]->LinkedTo.Num() > 0) {
		UGameFrameworkGraphNode* ChildNode = Cast<UGameFrameworkGraphNode>(RootNode->Pins[0]->LinkedTo[0]->GetOwningNode());
		UQuest* Quest = Cast<UQuest>(ChildNode->NodeInstance);

		auto QuestTree = Cast<UQuestTree>(Editor->GetObjectToEdit());
		QuestTree->Root = Quest;

		UpdateQuestNodeRecusive(ChildNode);
	}
}

void UQuestTreeGraph::UpdateQuestNodeRecusive(UGameFrameworkGraphNode* Node)
{
	UQuest* Quest = Cast<UQuest>(Node->NodeInstance);
	for (int PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex) {
		auto Pin = Node->Pins[PinIndex];
		if (Pin->Direction == EGPD_Output) {
			auto OutputPin = Pin;
			if (OutputPin != nullptr && OutputPin->LinkedTo.Num() != 0) {
				for (int32 LinkToIndex = 0; LinkToIndex < OutputPin->LinkedTo.Num(); LinkToIndex++) {
					UGameFrameworkGraphNode* ChildNode = Cast<UGameFrameworkGraphNode>(OutputPin->LinkedTo[LinkToIndex]->GetOwningNode());
					UQuest* QuestChild = Cast<UQuest>(ChildNode->NodeInstance);
					Quest->FollowQuests.Add(QuestChild);

					UpdateQuestNodeRecusive(ChildNode);
				}
			}
		}
	}
}