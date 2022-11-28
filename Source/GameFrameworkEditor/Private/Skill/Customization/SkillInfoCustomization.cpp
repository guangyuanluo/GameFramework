// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillInfoCustomization.h"
#include "Modules/Skill/SkillInfo.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SRowTableRefBox.h"
#include "Modules/Skill/SkillSetting.h"

TSharedRef<IPropertyTypeCustomization> FSkillInfoCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FSkillInfoCustomization());
}

void FSkillInfoCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    
}

void FSkillInfoCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
    if (SkillDataTable == nullptr) {
        return;
    }

    TSharedPtr<IPropertyHandle> SkillLevelPropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillInfo, SkillLevel));

    SkillIdPropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillInfo, SkillId));

    check(SkillLevelPropertyHandle.IsValid() && SkillIdPropertyHandle.IsValid());

    int CurSkillId;
    SkillIdPropertyHandle->GetValue(CurSkillId);
    
    TSharedPtr<SRowTableRefBox> SkillTableRefBox = SNew(SRowTableRefBox, SkillDataTable, CurSkillId);
    SkillTableRefBox->RowSelectChanged.BindSP(this, &FSkillInfoCustomization::OnSkillIdChange);

    StructBuilder.AddCustomRow(FText::FromString(TEXT("SkillInfo")))
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
                SkillLevelPropertyHandle->CreatePropertyNameWidget()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(0.65)
            [
                SkillLevelPropertyHandle->CreatePropertyValueWidget()
            ]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(0.35)
            [
                SkillIdPropertyHandle->CreatePropertyNameWidget()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(0.65)
            [
                SkillTableRefBox.ToSharedRef()
            ]
		]
	];
}

void FSkillInfoCustomization::OnSkillIdChange(int SkillId) {
    if (SkillIdPropertyHandle.IsValid()) {
        SkillIdPropertyHandle->SetValue(SkillId);
    }
}