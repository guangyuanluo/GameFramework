// Fill out your copyright notice in the Description page of Project Settings.

#include "Customization/ScenarioNodeCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "PropertyRestriction.h"

#include "Modules/Scenario/ScenarioNode.h"
#include "Modules/Scenario/ScenarioAction.h"
#include "SScenarionActionListWidget.h"

void FScenarioNodeCustomization::CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) {
	ActionsProperty  = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UScenarioNode, Actions));

	LayoutBuilder.HideProperty("Actions");
	IDetailCategoryBuilder& Category = LayoutBuilder.EditCategory("Actions", FText(), ECategoryPriority::Default);

	TSharedPtr<SScenarionActionListWidget> ScenarionActionListWidget = SNew(SScenarionActionListWidget, ActionsProperty);

	Category.AddCustomRow(FText())
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("动作列表")))
			.Font(LayoutBuilder.GetDetailFont())
		]
		.ValueContent()
		.HAlign(HAlign_Fill)
		[
			ScenarionActionListWidget.ToSharedRef()
		];
}

TSharedRef<IDetailCustomization> FScenarioNodeCustomization::MakeInstance() {
	return MakeShareable(new FScenarioNodeCustomization());
}