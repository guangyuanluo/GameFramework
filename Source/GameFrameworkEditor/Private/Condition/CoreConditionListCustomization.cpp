// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreConditionListCustomization.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionList.h"
#include "SConditionEditWidget.h"

TSharedRef<IPropertyTypeCustomization> FCoreConditionListCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FCoreConditionListCustomization());
}

void FCoreConditionListCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
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

void FCoreConditionListCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    //条件
	ConditionsProperty =
		StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FCoreConditionList, Conditions));

	TArray<UPackage*> OutPackages;
	ConditionsProperty->GetOuterPackages(OutPackages);
	
	TSharedPtr<SConditionEditWidget> ConditionEditWidget = SNew(SConditionEditWidget, OutPackages[0])
		.OnConditionChange_Raw(this, &FCoreConditionListCustomization::OnAssetChange);

	void* ConditionsPropertyValuePtr;
	ConditionsProperty->GetValueData(ConditionsPropertyValuePtr);
	TArray<UCoreCondition*>* ConditionsPtr = (TArray<UCoreCondition*>*)ConditionsPropertyValuePtr;
	ConditionEditWidget->RefreshConditionPtr(ConditionsPtr);

	StructBuilder.AddCustomRow(FText::FromString(TEXT("ConditionList")))
	[
		SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            StructPropertyHandle->CreatePropertyNameWidget()
        ]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(0.35)
            [
				ConditionsProperty->CreatePropertyNameWidget()
            ]
			+ SHorizontalBox::Slot()
			.FillWidth(0.65)
			[
				ConditionEditWidget.ToSharedRef()
            ]
		]
	];
}

void FCoreConditionListCustomization::OnAssetChange() {
	TArray<UPackage*> OutPackages;
	ConditionsProperty->GetOuterPackages(OutPackages);
	for (auto Package : OutPackages) {
		Package->MarkPackageDirty();
	}
}