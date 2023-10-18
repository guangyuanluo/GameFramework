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

void FCoreAbilityCustomization::CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) {
	auto SortPriorityProperty = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCoreAbility, SortPriority));

    LayoutBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UCoreAbility, SortPriority));

	TArray<TSharedRef<IPropertyHandle>> SimpleDefaultProperties;
	IDetailCategoryBuilder& CategoryBuilder = LayoutBuilder.EditCategory(SortPriorityProperty->GetDefaultCategoryName());
	CategoryBuilder.GetDefaultProperties(SimpleDefaultProperties, true, true);

	FName CategoryMetaData("Category");
	for (TSharedRef<IPropertyHandle> Property : SimpleDefaultProperties) {
		const FString& CategoryName = Property->GetMetaData(CategoryMetaData);

		IDetailCategoryBuilder& SimpleCategoryBuilder = LayoutBuilder.EditCategory(*CategoryName);

		if (Property->GetProperty()->GetFName() == GET_MEMBER_NAME_CHECKED(UCoreAbility, SortPriority)) {
			SimpleCategoryBuilder.AddCustomRow(SortPriorityProperty->GetPropertyDisplayName())
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
							.OnClicked_Lambda([](){
								const FVector2D BrowserWindowSize(1280, 720);
								TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
								FSlateApplication::Get().AddModalWindow(
									SNew(SWindow)
									.Title(FText::FromString(TEXT("技能全局优先级")))
									.HasCloseButton(true)
									.ClientSize(BrowserWindowSize)
									[
										SNew(SGlobalSkillInfoWidget)
									], RootWindow);
								return FReply::Handled();
							})
					]
				];
			continue;
		}

		if (Property->IsCustomized()) {
			continue;
		}
		IDetailPropertyRow& PropertyRow = SimpleCategoryBuilder.AddProperty(Property);
	}
}

TSharedRef<IDetailCustomization> FCoreAbilityCustomization::MakeInstance() {
	return MakeShareable(new FCoreAbilityCustomization());
}