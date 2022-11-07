// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestDetailFactory.h"

#include "Modules/Quest/QuestDetail.h"

UQuestDetailFactory::UQuestDetailFactory(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SupportedClass = UQuestDetail::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UQuestDetailFactory::CanCreateNew() const
{
	return true;
}

UObject* UQuestDetailFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UQuestDetail::StaticClass()));
	return NewObject<UQuestDetail>(InParent, Class, Name, Flags);;
}