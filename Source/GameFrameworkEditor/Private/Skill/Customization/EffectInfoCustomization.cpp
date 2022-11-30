// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectInfoCustomization.h"
#include "Modules/Skill/EffectInfo.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SRowTableRefBox.h"
#include "Modules/Skill/SkillSetting.h"

TSharedRef<IPropertyTypeCustomization> FEffectInfoCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FEffectInfoCustomization());
}

void FEffectInfoCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
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

void FEffectInfoCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto EffectDataTable = SkillSetting->EffectTable.LoadSynchronous();
    if (EffectDataTable == nullptr) {
        return;
    }

    TSharedPtr<IPropertyHandle> EffectLevelPropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEffectInfo, EffectLevel));

    EffectIdPropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEffectInfo, EffectID));

    check(EffectLevelPropertyHandle.IsValid() && EffectIdPropertyHandle.IsValid());

    int CurEffectId;
    EffectIdPropertyHandle->GetValue(CurEffectId);
    
    TSharedPtr<SRowTableRefBox> EffectTableRefBox = SNew(SRowTableRefBox, EffectDataTable, CurEffectId);
    EffectTableRefBox->RowSelectChanged.BindSP(this, &FEffectInfoCustomization::OnIdChange);

    StructBuilder.AddCustomRow(FText::FromString(TEXT("EffectInfo")))
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
                EffectLevelPropertyHandle->CreatePropertyNameWidget()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(0.65)
            [
                EffectLevelPropertyHandle->CreatePropertyValueWidget()
            ]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(0.35)
            [
                EffectIdPropertyHandle->CreatePropertyNameWidget()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(0.65)
            [
                EffectTableRefBox.ToSharedRef()
            ]
		]
	];
}

void FEffectInfoCustomization::OnIdChange(int SkillId) {
    if (EffectIdPropertyHandle.IsValid()) {
        EffectIdPropertyHandle->SetValue(SkillId);
    }
}