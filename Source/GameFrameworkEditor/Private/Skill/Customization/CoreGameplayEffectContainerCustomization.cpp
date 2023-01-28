// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreGameplayEffectContainerCustomization.h"
#include "Modules/Skill/CoreAbilityTypes.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailPropertyRow.h"
#include "IPropertyUtilities.h"

TSharedRef<IPropertyTypeCustomization> FCoreGameplayEffectContainerCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FCoreGameplayEffectContainerCustomization());
}

void FCoreGameplayEffectContainerCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    HeaderRow
	.NameContent()
	[
        StructPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Left)
	[
        StructPropertyHandle->CreatePropertyValueWidget(/*bDisplayDefaultPropertyButtons =*/false)
	];
}

void FCoreGameplayEffectContainerCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    
    uint32 NumChildren;
    StructPropertyHandle->GetNumChildren(NumChildren);

    auto ShowCounterEditProperty = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FCoreGameplayEffectContainer, ShowCounterEdit));
    bool ShowCounterEdit;
    ShowCounterEditProperty->GetValue(ShowCounterEdit);

    auto CounterEnumProperty = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FCoreGameplayEffectContainer, CounterEnum));
    uint8 CounterEnumRaw;
    CounterEnumProperty->GetValue(CounterEnumRaw);
    CoreAbilityCounterEnum CounterEnum = (CoreAbilityCounterEnum)CounterEnumRaw;
    CounterEnumProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([&StructCustomizationUtils]() {
        auto CachedUtils = StructCustomizationUtils.GetPropertyUtilities();
        CachedUtils->ForceRefresh();
    }));

    for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex) {
        TSharedRef<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();
        const FName PropertyName = ChildHandle->GetProperty()->GetFName();

        if (PropertyName == GET_MEMBER_NAME_CHECKED(FCoreGameplayEffectContainer, ShowCounterEdit)) {

        }
        else if (PropertyName == GET_MEMBER_NAME_CHECKED(FCoreGameplayEffectContainer, CounterEnum)) {
            if (ShowCounterEdit) {
                StructBuilder.AddProperty(ChildHandle).ShowPropertyButtons(true);
            }
        }
        else if(PropertyName == GET_MEMBER_NAME_CHECKED(FCoreGameplayEffectContainer, FixedCounter)) {
            if (ShowCounterEdit && CounterEnum == CoreAbilityCounterEnum::E_Fixed) {
                StructBuilder.AddProperty(ChildHandle).ShowPropertyButtons(true);
            }
        }
        else {
            StructBuilder.AddProperty(ChildHandle).ShowPropertyButtons(true);
        }
    }
}