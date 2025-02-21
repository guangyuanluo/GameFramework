// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreAbilityCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "PropertyRestriction.h"
#include "DetailWidgetRow.h"

#include "Modules/Skill/CoreAbility.h"
#include "SGlobalSkillInfoWidget.h"
#include "PropertyCustomizationHelpers.h"

void FCoreAbilityCustomization::CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) {
	TArray< TWeakObjectPtr<UObject> > OutObjects;
	LayoutBuilder.GetObjectsBeingCustomized(OutObjects);

	auto ComplexAbility = Cast<UCoreAbility>(OutObjects[0]);

	auto SortPriorityProperty = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCoreAbility, SortPriority));

    LayoutBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UCoreAbility, SortPriority));

	IDetailCategoryBuilder& CategoryBuilder = LayoutBuilder.EditCategory(SortPriorityProperty->GetDefaultCategoryName());
	CategoryBuilder.AddCustomRow(SortPriorityProperty->GetPropertyDisplayName())
		.NameContent()
		[
			SortPriorityProperty->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SortPriorityProperty->CreatePropertyValueWidget()
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
					.Text(FText::FromString(TEXT("查看全局优先级")))
					.OnClicked_Lambda([ComplexAbility](){
						const FVector2D BrowserWindowSize(1280, 720);
						TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
						FSlateApplication::Get().AddModalWindow(
							SNew(SWindow)
							.Title(FText::FromString(TEXT("技能全局优先级")))
							.HasCloseButton(true)
							.ClientSize(BrowserWindowSize)
							[
								SNew(SGlobalSkillInfoWidget, ComplexAbility->GetClass())
							], RootWindow);
						return FReply::Handled();
					})
			]
		];
}

TSharedRef<IDetailCustomization> FCoreAbilityCustomization::MakeInstance() {
	return MakeShareable(new FCoreAbilityCustomization());
}