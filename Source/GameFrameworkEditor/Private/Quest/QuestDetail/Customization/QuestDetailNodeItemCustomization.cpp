// Fill out your copyright notice in the Description page of Project Settings.

#include "Customization/QuestDetailNodeItemCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "PropertyRestriction.h"

#include "Modules/Quest/QuestDetailNodeItem.h"
#include "SConditionEditWidget.h"
#include "SSubClassObjectListWidget.h"
#include "Modules/Reward/CoreReward.h"

void FQuestDetailNodeItemCustomization::CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) {
	//条件
	ConditionProperty  = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UQuestDetailNodeItem, Conditions));

	TArray<UPackage*> OutPackages;
	ConditionProperty->GetOuterPackages(OutPackages);
	
	LayoutBuilder.HideProperty("Conditions");
	IDetailCategoryBuilder& ConditionCategory = LayoutBuilder.EditCategory("Conditions", FText(), ECategoryPriority::Default);

	TSharedPtr<SConditionEditWidget> ConditionEditWidget = SNew(SConditionEditWidget, OutPackages[0])
		.OnConditionChange_Raw(this, &FQuestDetailNodeItemCustomization::OnAssetChange);

	ConditionCategory.AddCustomRow(FText())
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("条件")))
			.Font(LayoutBuilder.GetDetailFont())
		]
		.ValueContent()
		.HAlign(HAlign_Fill)
		[
			ConditionEditWidget.ToSharedRef()
		];

	void* ConditionPropertyValuePtr;
	ConditionProperty->GetValueData(ConditionPropertyValuePtr);
	TArray<UCoreCondition*>* ConditionPtr = (TArray<UCoreCondition*>*)ConditionPropertyValuePtr;
	ConditionEditWidget->RefreshConditionPtr(ConditionPtr);

	//奖励
	RewardProperty  = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UQuestDetailNodeItem, Rewards));
	
	LayoutBuilder.HideProperty("Rewards");
	IDetailCategoryBuilder& RewardCategory = LayoutBuilder.EditCategory("Rewards", FText(), ECategoryPriority::Default);

	TSharedPtr<SSubClassObjectListWidget<UCoreReward>> RewardEditWidget = SNew(SSubClassObjectListWidget<UCoreReward>, RewardProperty);

	RewardCategory.AddCustomRow(FText())
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("奖励")))
			.Font(LayoutBuilder.GetDetailFont())
		]
		.ValueContent()
		.HAlign(HAlign_Fill)
		[
			RewardEditWidget.ToSharedRef()
		];

	void* RewardPropertyValuePtr;
	RewardProperty->GetValueData(RewardPropertyValuePtr);
	TArray<UCoreReward*>* RewardPtr = (TArray<UCoreReward*>*)RewardPropertyValuePtr;
	RewardEditWidget->RefreshPropertyHandle(RewardProperty);
}

TSharedRef<IDetailCustomization> FQuestDetailNodeItemCustomization::MakeInstance() {
	return MakeShareable(new FQuestDetailNodeItemCustomization());
}

void FQuestDetailNodeItemCustomization::OnAssetChange() {
	TArray<UPackage*> OutPackages;
	ConditionProperty->GetOuterPackages(OutPackages);
	for (auto Package : OutPackages) {
		Package->MarkPackageDirty();
	}
}