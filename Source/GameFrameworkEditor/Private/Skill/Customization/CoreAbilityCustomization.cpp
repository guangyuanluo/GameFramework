// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreAbilityCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "PropertyRestriction.h"

#include "Modules/Skill/CoreAbility.h"

void FCoreAbilityCustomization::CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) {
    
}

TSharedRef<IDetailCustomization> FCoreAbilityCustomization::MakeInstance() {
	return MakeShareable(new FCoreAbilityCustomization());
}