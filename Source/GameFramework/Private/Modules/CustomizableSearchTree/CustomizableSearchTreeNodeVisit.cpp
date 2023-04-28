// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomizableSearchTreeNodeVisit.h"

void UCustomizableSearchTreeNodeVisit::VisitNode(class UCustomizableSearchTreeNodeBase* Node, FCustomizableSearchTreeNodeVisitFunc VisitNodeFunc, bool& NeedSearchChildren, bool& NeedBreakLoop) {
	if (VisitNodeFunc.IsBound()) {
		VisitNodeFunc.Execute(Node, NeedSearchChildren, NeedBreakLoop);
	}
}