// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenarioNode.h"

UScenarioNode::UScenarioNode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

class UScenario* UScenarioNode::GetScenarioAsset() const {
    return ScenarioAsset;
}

const TArray<class UScenarioNode*>& UScenarioNode::GetFollowScenarioNodes() const {
    return FollowScenarioNodes;
}

FString UScenarioNode::GetNodeTitle_Implementation() {
    return TEXT("");
}

TArray<FScenarioNodeOutputPinInfo> UScenarioNode::GetOutputPinInfo_Implementation() {
    return TArray<FScenarioNodeOutputPinInfo>();
}