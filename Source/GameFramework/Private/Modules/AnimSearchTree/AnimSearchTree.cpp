// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/AnimSearchTree/AnimSearchTree.h"
#include "Modules/AnimSearchTree/AnimSearchTreeNodeBase.h"

UAnimSearchTree::UAnimSearchTree(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    LimitNodeClass = UAnimSearchTreeNodeBase::StaticClass();
}