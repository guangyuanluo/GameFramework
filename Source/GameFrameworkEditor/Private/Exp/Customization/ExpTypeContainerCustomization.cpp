// Fill out your copyright notice in the Description page of Project Settings.

#include "ExpTypeContainerCustomization.h"
#include "Modules/Exp/ExpTypeContainer.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SRowTableRefBox.h"
#include "Modules/Exp/ExpSetting.h"

TSharedRef<IPropertyTypeCustomization> FExpTypeContainerCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FExpTypeContainerCustomization());
}

void FExpTypeContainerCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
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

void FExpTypeContainerCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
    auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();
    if (ExpTypeDataTable == nullptr) {
        return;
    }

    ExpTypePropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FExpTypeContainer, ExpType));

    check(ExpTypePropertyHandle.IsValid());

    int CurExpType;
    ExpTypePropertyHandle->GetValue(CurExpType);
    
    TSharedPtr<SRowTableRefBox> TableRefBox = SNew(SRowTableRefBox, ExpTypeDataTable, CurExpType);
    TableRefBox->RowSelectChanged.BindSP(this, &FExpTypeContainerCustomization::OnIDPropertyChange);

    StructBuilder.AddCustomRow(FText::FromString(TEXT("ExpTypeNum")))
	[
		SNew(SVerticalBox)
        + SVerticalBox::Slot()
		.AutoHeight()
		[
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(0.35)
            [
                ExpTypePropertyHandle->CreatePropertyNameWidget()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(0.65)
            [
                TableRefBox.ToSharedRef()
            ]
		]
	];
}

void FExpTypeContainerCustomization::OnIDPropertyChange(int NewID) {
    if (ExpTypePropertyHandle.IsValid()) {
        ExpTypePropertyHandle->SetValue(NewID);
    }
}