// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestTreeFactory.h"

#include "Modules/Quest/QuestTree.h"

UQuestTreeFactory::UQuestTreeFactory(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SupportedClass = UQuestTree::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UQuestTreeFactory::CanCreateNew() const
{
	return true;
}

UObject* UQuestTreeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UQuestTree::StaticClass()));
	return NewObject<UQuestTree>(InParent, Class, Name, Flags);;
}