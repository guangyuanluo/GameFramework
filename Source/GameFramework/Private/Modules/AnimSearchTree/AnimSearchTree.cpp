// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimSearchTree.h"
#include "AnimSearchTreeNodeBase.h"

UAnimSearchTree::UAnimSearchTree(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    LimitNodeClass = UAnimSearchTreeNodeBase::StaticClass();
}