// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphPanelNodeFactoryCommon.h"
#include "SGameGraphNode.h"
#include "GameFrameworkGraphNode.h"

TSharedPtr<class SGraphNode> FGraphPanelNodeFactoryCommon::CreateNode(UEdGraphNode* Node) const {
	if (Cast<UGameFrameworkGraphNode>(Node)) {
		return SNew(SGameGraphNode, Node);
	}
	return NULL;
}