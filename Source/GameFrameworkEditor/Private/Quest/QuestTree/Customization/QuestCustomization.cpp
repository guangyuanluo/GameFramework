// Fill out your copyright notice in the Description page of Project Settings.

#include "Customization/QuestCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "PropertyRestriction.h"

#include "Modules/Quest/Quest.h"
#include "SConditionEditWidget.h"

void FQuestCustomization::CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) {
	
}

TSharedRef<IDetailCustomization> FQuestCustomization::MakeInstance() {
	return MakeShareable(new FQuestCustomization());
}