// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestDetailNodeItem.h"
#include "CoreCondition.h"
#include "CoreReward.h"

UQuestDetailNodeItem::UQuestDetailNodeItem(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    
}

FString UQuestDetailNodeItem::GetNodeTitle_Implementation() {
    return Name;
}

FString UQuestDetailNodeItem::GetNodeTypeName_Implementation() {
    return TEXT("任务子项");
}