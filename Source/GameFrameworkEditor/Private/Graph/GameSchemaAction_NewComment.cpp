// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Graph/GameSchemaAction_NewComment.h"
#include "Graph/GameFrameworkGraph.h"
#include "EdGraphNode_Comment.h"
#include "Graph/GraphEditorBase.h"
#include "ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "Graph"

UEdGraphNode* FGameSchemaAction_NewComment::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /*= true*/)
{
	UGameFrameworkGraph* EditorGraph = Cast<UGameFrameworkGraph>(ParentGraph);
	if (!EditorGraph)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid EditorGraph"));
		return nullptr;
	}

	UEdGraphNode_Comment* CommentTemplate = NewObject<UEdGraphNode_Comment>();
	auto GraphEditorPtr = EditorGraph->Editor;

	FVector2D SpawnLocation = Location;
	FSlateRect Bounds;
	if (GraphEditorPtr && GraphEditorPtr->GetBoundsForSelectedNodes(Bounds, 50.0f))
	{
		CommentTemplate->SetBounds(Bounds);
		SpawnLocation.X = CommentTemplate->NodePosX;
		SpawnLocation.Y = CommentTemplate->NodePosY;
	}

	const FScopedTransaction Transaction(NSLOCTEXT("FGameSchemaAction_NewComment", "NewComment", "NewComment"));
	EditorGraph->Modify();

	UEdGraphNode_Comment* NewNode = FEdGraphSchemaAction_NewNode::SpawnNodeFromTemplate<UEdGraphNode_Comment>(ParentGraph, CommentTemplate, SpawnLocation, bSelectNewNode);

	return NewNode;
}

#undef LOCTEXT_NAMESPACE
