// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Graph/GameFrameworkGraph.h"
#include "Graph/GameFrameworkGraphSchema.h"
#include "Graph/GameFrameworkGraphNode.h"

UGameFrameworkGraph::UGameFrameworkGraph(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Schema = UGameFrameworkGraphSchema::StaticClass();
}

void UGameFrameworkGraph::RefreshAllNodesOnDataChanged() {
	for (UEdGraphNode* node : Nodes) {
		UGameFrameworkGraphNode* GraphNode = Cast<UGameFrameworkGraphNode>(node);
		if (GraphNode) {
			GraphNode->RefreshNodeOnDataChanged();
		}
	}
	NotifyGraphChanged();
}