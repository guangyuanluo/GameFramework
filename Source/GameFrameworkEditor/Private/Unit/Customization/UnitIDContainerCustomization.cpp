// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitIDContainerCustomization.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SRowTableRefBox.h"
#include "Modules/Unit/UnitSetting.h"

TSharedRef<IPropertyTypeCustomization> FUnitIDContainerCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FUnitIDContainerCustomization());
}

void FUnitIDContainerCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
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

void FUnitIDContainerCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
    if (UnitDataTable == nullptr) {
        return;
    }

    UnitIDPropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FUnitIDContainer, UnitID));

    check(UnitIDPropertyHandle.IsValid());

    int CurUnitID;
    UnitIDPropertyHandle->GetValue(CurUnitID);
    
    TSharedPtr<SRowTableRefBox> UnitTableRefBox = SNew(SRowTableRefBox, UnitDataTable, CurUnitID);
    UnitTableRefBox->RowSelectChanged.BindSP(this, &FUnitIDContainerCustomization::OnUnitIDChange);

    StructBuilder.AddCustomRow(FText::FromString(TEXT("Unit")))
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
                UnitIDPropertyHandle->CreatePropertyNameWidget()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(0.65)
            [
                UnitTableRefBox.ToSharedRef()
            ]
		]
	];
}

void FUnitIDContainerCustomization::OnUnitIDChange(int UnitID) {
    if (UnitIDPropertyHandle.IsValid()) {
        UnitIDPropertyHandle->SetValue(UnitID);
    }
}