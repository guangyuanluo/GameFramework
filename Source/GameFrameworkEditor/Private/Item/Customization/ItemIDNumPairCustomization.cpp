// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemIDNumPairCustomization.h"
#include "Modules/Item/ItemIDNumPair.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SRowTableRefBox.h"
#include "Modules/Item/ItemSetting.h"

TSharedRef<IPropertyTypeCustomization> FItemIDNumPairCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FItemIDNumPairCustomization());
}

void FItemIDNumPairCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
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

void FItemIDNumPairCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    if (ItemDataTable == nullptr) {
        return;
    }

    ItemIDPropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FItemIDNumPair, ItemID));

    TSharedPtr<IPropertyHandle> ItemNumPropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FItemIDNumPair, ItemNum));

    check(ItemIDPropertyHandle.IsValid() && ItemNumPropertyHandle.IsValid());

    int CurItemID;
    ItemIDPropertyHandle->GetValue(CurItemID);
    
    TSharedPtr<SRowTableRefBox> TableRefBox = SNew(SRowTableRefBox, ItemDataTable, CurItemID);
    TableRefBox->RowSelectChanged.BindSP(this, &FItemIDNumPairCustomization::OnIDPropertyChange);

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
                ItemIDPropertyHandle->CreatePropertyNameWidget()
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
                ItemNumPropertyHandle->CreatePropertyNameWidget()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(0.65)
            [
                ItemNumPropertyHandle->CreatePropertyValueWidget()
            ]
		]
		
	];
}

void FItemIDNumPairCustomization::OnIDPropertyChange(int NewID) {
    if (ItemIDPropertyHandle.IsValid()) {
        ItemIDPropertyHandle->SetValue(NewID);
    }
}