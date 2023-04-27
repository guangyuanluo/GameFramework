// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomizableSearchTreeNodeChecker.h"

class UCustomizableSearchTreeNodeBase* UCustomizableSearchTreeNodeChecker::FindNode_Implementation(class UCustomizableSearchTreeNodeBase* Node, TSubclassOf<class UCustomizableSearchTreeNodeBase> NodeClass, UObject* FindContext) {
    return nullptr;
}

bool UCustomizableSearchTreeNodeChecker::NeedSearchChildren_Implementation(class UCustomizableSearchTreeNodeBase* Node, UObject* FindContext) {
    return true;
}