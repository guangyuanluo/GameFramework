// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimSearchTreeFactory.h"

#include "Modules/AnimSearchTree/AnimSearchTree.h"

UAnimSearchTreeFactory::UAnimSearchTreeFactory(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SupportedClass = UAnimSearchTree::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UAnimSearchTreeFactory::CanCreateNew() const
{
	return true;
}

UObject* UAnimSearchTreeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UAnimSearchTree::StaticClass()));
	return NewObject<UAnimSearchTree>(InParent, Class, Name, Flags);;
}