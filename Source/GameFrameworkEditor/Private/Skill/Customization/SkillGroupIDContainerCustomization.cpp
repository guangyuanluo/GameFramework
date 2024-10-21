// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillGroupIDContainerCustomization.h"
#include "Modules/Skill/SkillGroupIDContainer.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "SRowTableRefBox.h"
#include "Modules/Skill/SkillSetting.h"

TSharedRef<IPropertyTypeCustomization> FSkillGroupIDContainerCustomization::MakeInstance() {
    // Create the instance and returned a SharedRef
    return MakeShareable(new FSkillGroupIDContainerCustomization());
}

void FSkillGroupIDContainerCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
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

void FSkillGroupIDContainerCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
    class IDetailChildrenBuilder& StructBuilder,
    IPropertyTypeCustomizationUtils& StructCustomizationUtils) {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto SkillGroupTable = SkillSetting->SkillGroupTable.LoadSynchronous();
    if (SkillGroupTable == nullptr) {
        return;
    }

    SkillGroupIDPropertyHandle =
        StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillGroupIDContainer, SkillGroupID));

    check(SkillGroupIDPropertyHandle.IsValid());

    int CurSkillGroupID;
    SkillGroupIDPropertyHandle->GetValue(CurSkillGroupID);
    
    TSharedPtr<SRowTableRefBox> SkillGroupTableRefBox = SNew(SRowTableRefBox, SkillGroupTable, CurSkillGroupID);
    SkillGroupTableRefBox->RowSelectChanged.BindSP(this, &FSkillGroupIDContainerCustomization::OnSkillGroupIDChange);

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
                SkillGroupIDPropertyHandle->CreatePropertyNameWidget()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(0.65)
            [
                SkillGroupTableRefBox.ToSharedRef()
            ]
		]
	];
}

void FSkillGroupIDContainerCustomization::OnSkillGroupIDChange(int SkillGroupID) {
    if (SkillGroupIDPropertyHandle.IsValid()) {
        SkillGroupIDPropertyHandle->SetValue(SkillGroupID);
    }
}