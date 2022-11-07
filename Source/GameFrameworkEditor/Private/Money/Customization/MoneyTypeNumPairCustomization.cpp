// Fill out your copyright notice in the Description page of Project Settings.

#include "MoneyTypeNumPairCustomization.h"
#include "Modules/Money/MoneyTypeNumPair.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SRowTableRefBox.h"
#include "Modules/Money/MoneySetting.h"

TSharedRef<IPropertyTypeCustomization> FMoneyTypeNumPairCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FMoneyTypeNumPairCustomization());
}

void FMoneyTypeNumPairCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    
}

void FMoneyTypeNumPairCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    const UMoneySetting* MoneySetting = GetDefault<UMoneySetting>();
    auto MoneyTypeDataTable = MoneySetting->MoneyTypeTable.LoadSynchronous();
    if (MoneyTypeDataTable == nullptr) {
        return;
    }

    MoneyTypePropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMoneyTypeNumPair, MoneyType));

    TSharedPtr<IPropertyHandle> MoneyNumPropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMoneyTypeNumPair, Num));

    check(MoneyTypePropertyHandle.IsValid() && MoneyNumPropertyHandle.IsValid());

    int CurMoneyType;
    MoneyTypePropertyHandle->GetValue(CurMoneyType);
    
    TSharedPtr<SRowTableRefBox> TableRefBox = SNew(SRowTableRefBox, MoneyTypeDataTable, CurMoneyType);
    TableRefBox->RowSelectChanged.BindSP(this, &FMoneyTypeNumPairCustomization::OnIDPropertyChange);

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
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(0.35)
            [
                MoneyNumPropertyHandle->CreatePropertyNameWidget()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(0.65)
            [
                MoneyNumPropertyHandle->CreatePropertyValueWidget()
            ]
		]
		
	];
}

void FMoneyTypeNumPairCustomization::OnIDPropertyChange(int NewID) {
    if (MoneyTypePropertyHandle.IsValid()) {
        MoneyTypePropertyHandle->SetValue(NewID);
    }
}