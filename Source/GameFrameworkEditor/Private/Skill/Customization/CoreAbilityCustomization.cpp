// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreAbilityCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "PropertyRestriction.h"

#include "Modules/Skill/CoreAbility.h"

void FCoreAbilityCustomization::CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) {
    auto LimitActiveCounterProperty = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCoreAbility, LimitActiveCounter));

    int LimitActiveCounter;
    LimitActiveCounterProperty->GetValue(LimitActiveCounter);
    bool ShowCounterEdit = LimitActiveCounter != 0;

    auto EffectContainerMapProperty = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCoreAbility, EffectContainerMap));
    void* DataPtr;
    EffectContainerMapProperty->GetValueData(DataPtr);
    TMap<FGameplayTag, FCoreGameplayEffectContainer>* MapPtr = (TMap<FGameplayTag, FCoreGameplayEffectContainer>*)DataPtr;
    for (auto Iter = MapPtr->CreateIterator(); Iter; ++Iter) {
        FCoreGameplayEffectContainer& Value = Iter->Value;
        Value.ShowCounterEdit = ShowCounterEdit;
    }

    LimitActiveCounterProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([&LayoutBuilder]() {
        LayoutBuilder.ForceRefreshDetails();
    }));
}

TSharedRef<IDetailCustomization> FCoreAbilityCustomization::MakeInstance() {
	return MakeShareable(new FCoreAbilityCustomization());
}