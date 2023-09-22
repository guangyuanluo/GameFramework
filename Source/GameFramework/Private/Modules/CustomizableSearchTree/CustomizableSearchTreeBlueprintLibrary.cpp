// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomizableSearchTreeBlueprintLibrary.h"
#include "CustomizableSearchTree.h"
#include "CustomizableSearchTreeNodeBase.h"
#include "CustomizableSearchTreeNodeVisit.h"

UCustomizableSearchTreeNodeBase* UCustomizableSearchTreeBlueprintLibrary::FindNodeFromAnimSearchTree(UCustomizableSearchTree* CustomizableSearchTree, TSubclassOf<UCustomizableSearchTreeNodeBase> NodeClass, UObject* FindContext) {
	if (!NodeClass) {
		return nullptr;
	}
	//遍历
	UCustomizableSearchTreeNodeBase* FindResult = nullptr;

	FCustomizableSearchTreeNodeVisitFunc VisitFunc;
	VisitFunc.BindLambda([FindContext, &FindResult, NodeClass](class UCustomizableSearchTreeNodeBase* Node, bool& NeedSearchChildren, bool& NeedBreakLoop) {
		if (Node->GetClass() == NodeClass) {
			FindResult = Node;
			NeedBreakLoop = true;
		}
		else {
			NeedBreakLoop = false;
			NeedSearchChildren = Node->NeedSearchChildren(FindContext);
		}
	});

	bool NeedBreakLoop;
	UCustomizableSearchTreeBlueprintLibrary::VisitAnimSearchTree(CustomizableSearchTree, VisitFunc, NeedBreakLoop);

	return FindResult;
}

void UCustomizableSearchTreeBlueprintLibrary::FindAllNodesByClassFromAnimSearchTree(class UCustomizableSearchTree* CustomizableSearchTree, TSubclassOf<class UCustomizableSearchTreeNodeBase> NodeClass, TArray<UCustomizableSearchTreeNodeBase*>& OutNodes) {
	if (!NodeClass) {
		return;
	}
	TArray<UCustomizableSearchTreeNodeBase*> VisitStack;
	for (int Index = CustomizableSearchTree->Root->FollowNodes.Num() - 1; Index >= 0; --Index) {
		VisitStack.Add(CustomizableSearchTree->Root->FollowNodes[Index]);
	}

	//遍历	
	FCustomizableSearchTreeNodeVisitFunc VisitFunc;
	VisitFunc.BindLambda([&VisitStack, &OutNodes, NodeClass](class UCustomizableSearchTreeNodeBase* Node, bool& NeedSearchChildren, bool& NeedBreakLoop) {
		if (Node->GetClass() == NodeClass) {
			OutNodes.Add(Node);
		}
		NeedSearchChildren = true;
		NeedBreakLoop = false;
	});

	bool NeedBreakLoop;
	UCustomizableSearchTreeBlueprintLibrary::VisitAnimSearchTree(CustomizableSearchTree, VisitFunc, NeedBreakLoop);
}

void UCustomizableSearchTreeBlueprintLibrary::VisitAnimSearchTree(class UCustomizableSearchTree* CustomizableSearchTree, FCustomizableSearchTreeNodeVisitFunc VisitFunc, bool& NeedBreakLoop) {
	if (!IsValid(CustomizableSearchTree)) {
		return;
	}
	if (!IsValid(CustomizableSearchTree->Root)) {
		return;
	}
	if (CustomizableSearchTree->Root->FollowNodes.Num() == 0) {
		return;
	}
	TSet<UCustomizableSearchTreeNodeBase*> NodeSet;
	TArray<UCustomizableSearchTreeNodeBase*> VisitStack;
	for (int Index = CustomizableSearchTree->Root->FollowNodes.Num() - 1; Index >= 0; --Index) {
		auto Node = CustomizableSearchTree->Root->FollowNodes[Index];
		if (NodeSet.Contains(Node)) {
			continue;
		}
		NodeSet.Add(Node);
		VisitStack.Add(Node);
	}
	while (VisitStack.Num() > 0) {
		auto NowNode = VisitStack[VisitStack.Num() - 1];
		//出栈
		VisitStack.RemoveAt(VisitStack.Num() - 1);
		auto VisitClass = NowNode->GetVisitClass();
		ensure(VisitClass);
		if (!VisitClass) {
			continue;
		}
		auto VisitNodeCDO = VisitClass->GetDefaultObject<UCustomizableSearchTreeNodeVisit>();
		bool NeedSearchChildren;
		VisitNodeCDO->VisitNode(NowNode, VisitFunc, NeedSearchChildren, NeedBreakLoop);
		if (NeedBreakLoop) {
			break;
		}
		if (NeedSearchChildren && NowNode->FollowNodes.Num() > 0) {
			for (int ChildIndex = NowNode->FollowNodes.Num() - 1; ChildIndex >= 0; --ChildIndex) {
				auto Node = NowNode->FollowNodes[ChildIndex];
				if (NodeSet.Contains(Node)) {
					continue;
				}
				NodeSet.Add(Node);
				VisitStack.Add(Node);
			}
		}
	}
}