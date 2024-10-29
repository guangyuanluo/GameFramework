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

	TArray<UObject*> OutObjects;
	ConditionsProperty->GetOuterObjects(OutObjects);
	
	TSharedPtr<SConditionEditWidget> ConditionEditWidget = SNew(SConditionEditWidget, OutObjects[0])
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
			ConditionsProperty->CreatePropertyNameWidget()
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			ConditionEditWidget.ToSharedRef()
		]
	];
}

void FCoreConditionListCustomization::OnAssetChange() {
	ConditionsProperty->NotifyPostChange(EPropertyChangeType::ValueSet);
	ConditionsProperty->NotifyFinishedChangingProperties();

	TArray<UPackage*> OutPackages;
	ConditionsProperty->GetOuterPackages(OutPackages);
	for (auto Package : OutPackages) {
		Package->MarkPackageDirty();
	}
}