// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomizableSearchTreeNodeBase.h"
#include "CustomizableSearchTreeNodeVisit.h"

FString UCustomizableSearchTreeNodeBase::GetNodeTitle_Implementation() {
#if WITH_EDITORONLY_DATA
    return GetClass()->GetDisplayNameText().ToString();
#else
    return GetClass()->GetName();
#endif
}

TArray<FCustomizableSearchTreeNodeOutputPinInfo> UCustomizableSearchTreeNodeBase::GetOutputPinInfo_Implementation() {
    FCustomizableSearchTreeNodeOutputPinInfo PinInfo;
    PinInfo.AllowMulti = true;
    PinInfo.Name = TEXT("Next");
    return TArray<FCustomizableSearchTreeNodeOutputPinInfo>({ PinInfo });
}

TSubclassOf<class UCustomizableSearchTreeNodeVisit> UCustomizableSearchTreeNodeBase::GetVisitClass_Implementation() {
    return UCustomizableSearchTreeNodeVisit::StaticClass();
}

bool UCustomizableSearchTreeNodeBase::NeedSearchChildren_Implementation(UObject* FindContext) {
    return false;
}