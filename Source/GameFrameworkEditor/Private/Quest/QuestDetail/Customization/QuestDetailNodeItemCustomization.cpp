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
	RewardProperty->GetOuterPackages(OutPackages);
	for (auto Package : OutPackages) {
		Package->MarkPackageDirty();
	}
}