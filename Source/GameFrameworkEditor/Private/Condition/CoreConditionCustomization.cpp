// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreConditionCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "PropertyRestriction.h"

#include "Modules/Condition/CoreCondition.h"

void FCoreConditionCustomization::CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) {
	LayoutBuilder.HideProperty("ID");
	LayoutBuilder.HideProperty("ProgressClass");
}

TSharedRef<IDetailCustomization> FCoreConditionCustomization::MakeInstance() {
	return MakeShareable(new FCoreConditionCustomization());
}