// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomizableSearchTreeNodeBase.h"
#include "CustomizableSearchTreeNodeVisit.h"

FString UCustomizableSearchTreeNodeBase::GetNodeTitle_Implementation() {
    return GetClass()->GetDisplayNameText().ToString();
}

TArray<FCustomizableSearchTreeNodeOutputPinInfo> UCustomizableSearchTreeNodeBase::GetOutputPinInfo_Implementation() {
    FCustomizableSearchTreeNodeOutputPinInfo PinInfo;
    PinInfo.AllowMulti = true;
    PinInfo.Name = "Next";
    return TArray<FCustomizableSearchTreeNodeOutputPinInfo>({ PinInfo });
}

TSubclassOf<class UCustomizableSearchTreeNodeVisit> UCustomizableSearchTreeNodeBase::GetVisitClass_Implementation() {
    return UCustomizableSearchTreeNodeVisit::StaticClass();
}

bool UCustomizableSearchTreeNodeBase::NeedSearchChildren_Implementation(UObject* FindContext) {
    return false;
}