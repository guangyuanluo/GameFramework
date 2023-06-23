// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemIDContainerCustomization.h"
#include "Modules/Item/ItemIDContainer.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SRowTableRefBox.h"
#include "Modules/Item/ItemSetting.h"

TSharedRef<IPropertyTypeCustomization> FItemIDContainerCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FItemIDContainerCustomization());
}

void FItemIDContainerCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
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

void FItemIDContainerCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    if (ItemDataTable == nullptr) {
        return;
    }

    ItemIDPropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FItemIDContainer, ItemID));

    check(ItemIDPropertyHandle.IsValid());

    int CurItemID;
    ItemIDPropertyHandle->GetValue(CurItemID);
    
    TSharedPtr<SRowTableRefBox> TableRefBox = SNew(SRowTableRefBox, ItemDataTable, CurItemID);
    TableRefBox->RowSelectChanged.BindSP(this, &FItemIDContainerCustomization::OnIDPropertyChange);

    StructBuilder.AddCustomRow(FText::FromString(TEXT("ItemID")))
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
	];
}

void FItemIDContainerCustomization::OnIDPropertyChange(int NewID) {
    if (ItemIDPropertyHandle.IsValid()) {
        ItemIDPropertyHandle->SetValue(NewID);
    }
}