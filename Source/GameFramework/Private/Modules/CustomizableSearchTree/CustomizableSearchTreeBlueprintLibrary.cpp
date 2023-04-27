// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomizableSearchTreeBlueprintLibrary.h"
#include "CustomizableSearchTree.h"
#include "CustomizableSearchTreeNodeBase.h"
#include "CustomizableSearchTreeNodeChecker.h"

UCustomizableSearchTreeNodeBase* UCustomizableSearchTreeBlueprintLibrary::FindNodeFromAnimSearchTree(UCustomizableSearchTree* CustomizableSearchTree, TSubclassOf<UCustomizableSearchTreeNodeBase> NodeClass, UObject* FindContext) {
    if (!IsValid(CustomizableSearchTree)) {
        return nullptr;
    }
    if (!IsValid(CustomizableSearchTree->Root)) {
        return nullptr;
    }
    if (CustomizableSearchTree->Root->FollowNodes.Num() == 0) {
        return nullptr;
    }
    if (!NodeClass) {
        return nullptr;
    }
    TArray<UCustomizableSearchTreeNodeBase*> VisitStack;
    for (int Index = CustomizableSearchTree->Root->FollowNodes.Num() - 1; Index >= 0; --Index) {
        VisitStack.Add(CustomizableSearchTree->Root->FollowNodes[Index]);
    }
    //遍历
    UCustomizableSearchTreeNodeBase* FindResult = nullptr;
    while (VisitStack.Num() > 0) {
        auto NowNode = VisitStack[VisitStack.Num() - 1];
        //出栈
        VisitStack.RemoveAt(VisitStack.Num() - 1);
        if (NowNode->GetClass() == NodeClass) {
            //找到，终止循环
            FindResult = NowNode;
            break;
        }
        else {
            auto CheckerClass = NowNode->GetCheckerClass();
            ensure(CheckerClass);
            if (!CheckerClass) {
                continue;
            }
            auto CheckerNodeCDO = CheckerClass->GetDefaultObject<UCustomizableSearchTreeNodeChecker>();
            auto FindNode = CheckerNodeCDO->FindNode(NowNode, NodeClass, FindContext);
            if (FindNode) {
                FindResult = FindNode;
                break;
            }
            //没找到就继续查找树
            if (NowNode->FollowNodes.Num() > 0) {
                //不是这个类型，要判断是否符合条件
                if (CheckerNodeCDO->NeedSearchChildren(NowNode, FindContext)) {
                    //这里说明节点满足条件，可以访问子节点
                    for (int ChildIndex = NowNode->FollowNodes.Num() - 1; ChildIndex >= 0; --ChildIndex) {
                        //入栈
                        VisitStack.Add(NowNode->FollowNodes[ChildIndex]);
                    }
                }
            }
        }
    }
    return FindResult;
}