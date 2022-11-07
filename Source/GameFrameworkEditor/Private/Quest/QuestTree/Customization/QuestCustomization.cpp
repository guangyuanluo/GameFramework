// Fill out your copyright notice in the Description page of Project Settings.

#include "Customization/QuestCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "PropertyRestriction.h"

#include "Modules/Quest/Quest.h"
#include "SConditionEditWidget.h"

void FQuestCustomization::CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) {
	PreConditionProperty  = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UQuest, PreConditions));

	TArray<UPackage*> OutPackages;
	PreConditionProperty->GetOuterPackages(OutPackages);

	LayoutBuilder.HideProperty("PreConditions");
	IDetailCategoryBuilder& Category = LayoutBuilder.EditCategory("PreConditions", FText(), ECategoryPriority::Default);

	TSharedPtr<SConditionEditWidget> ConditionEditWidget = SNew(SConditionEditWidget, OutPackages[0])
		.OnConditionChange_Raw(this, &FQuestCustomization::OnConditionChange);

	Category.AddCustomRow(FText())
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("前置条件")))
			.Font(LayoutBuilder.GetDetailFont())
		]
		.ValueContent()
		.HAlign(HAlign_Fill)
		[
			ConditionEditWidget.ToSharedRef()
		];

	void* PropertyValuePtr;
	PreConditionProperty->GetValueData(PropertyValuePtr);
	TArray<UCoreCondition*>* ConditionPtr = (TArray<UCoreCondition*>*)PropertyValuePtr;
	ConditionEditWidget->RefreshConditionPtr(ConditionPtr);
}

TSharedRef<IDetailCustomization> FQuestCustomization::MakeInstance() {
	return MakeShareable(new FQuestCustomization());
}

void FQuestCustomization::OnConditionChange() {
	TArray<UPackage*> OutPackages;
	PreConditionProperty->GetOuterPackages(OutPackages);
	for (auto Package : OutPackages) {
		Package->MarkPackageDirty();
	}
}