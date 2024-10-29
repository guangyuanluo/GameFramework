// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreTriggerActionListCustomization.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "Modules/TriggerAction/CoreTriggerAction.h"
#include "Modules/TriggerAction/CoreTriggerActionList.h"
#include "STriggerActionListWidget.h"

TSharedRef<IPropertyTypeCustomization> FCoreTriggerActionListCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FCoreTriggerActionListCustomization());
}

void FCoreTriggerActionListCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
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

void FCoreTriggerActionListCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    //动作
	ActionsProperty =
		StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FCoreTriggerActionList, Actions));

	TArray<UObject*> OutObjects;
	ActionsProperty->GetOuterObjects(OutObjects);
	
	TSharedPtr<STriggerActionListWidget> TriggerActionListWidget = SNew(STriggerActionListWidget, OutObjects[0])
		.OnActionChange_Raw(this, &FCoreTriggerActionListCustomization::OnAssetChange);

	void* ActionsPropertyValuePtr;
	ActionsProperty->GetValueData(ActionsPropertyValuePtr);
	TArray<UCoreTriggerAction*>* ActionsPtr = (TArray<UCoreTriggerAction*>*)ActionsPropertyValuePtr;
	TriggerActionListWidget->RefreshActionArrayPtr(ActionsPtr);

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
			ActionsProperty->CreatePropertyNameWidget()
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			TriggerActionListWidget.ToSharedRef()
		]
	];
}

void FCoreTriggerActionListCustomization::OnAssetChange() {
	ActionsProperty->NotifyPostChange(EPropertyChangeType::ValueSet);
	ActionsProperty->NotifyFinishedChangingProperties();

	TArray<UPackage*> OutPackages;
	ActionsProperty->GetOuterPackages(OutPackages);
	for (auto Package : OutPackages) {
		Package->MarkPackageDirty();
	}
}