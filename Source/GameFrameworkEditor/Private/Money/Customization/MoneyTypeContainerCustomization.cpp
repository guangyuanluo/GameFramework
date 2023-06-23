// Fill out your copyright notice in the Description page of Project Settings.

#include "MoneyTypeContainerCustomization.h"
#include "Modules/Money/MoneyTypeContainer.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SRowTableRefBox.h"
#include "Modules/Money/MoneySetting.h"

TSharedRef<IPropertyTypeCustomization> FMoneyTypeContainerCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FMoneyTypeContainerCustomization());
}

void FMoneyTypeContainerCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
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

void FMoneyTypeContainerCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    const UMoneySetting* MoneySetting = GetDefault<UMoneySetting>();
    auto MoneyTypeDataTable = MoneySetting->MoneyTypeTable.LoadSynchronous();
    if (MoneyTypeDataTable == nullptr) {
        return;
    }

    MoneyTypePropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMoneyTypeContainer, MoneyType));

    check(MoneyTypePropertyHandle.IsValid());

    int CurMoneyType;
    MoneyTypePropertyHandle->GetValue(CurMoneyType);
    
    TSharedPtr<SRowTableRefBox> TableRefBox = SNew(SRowTableRefBox, MoneyTypeDataTable, CurMoneyType);
    TableRefBox->RowSelectChanged.BindSP(this, &FMoneyTypeContainerCustomization::OnIDPropertyChange);

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
                MoneyTypePropertyHandle->CreatePropertyNameWidget()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(0.65)
            [
                TableRefBox.ToSharedRef()
            ]
		]
	];
}

void FMoneyTypeContainerCustomization::OnIDPropertyChange(int NewID) {
    if (MoneyTypePropertyHandle.IsValid()) {
        MoneyTypePropertyHandle->SetValue(NewID);
    }
}