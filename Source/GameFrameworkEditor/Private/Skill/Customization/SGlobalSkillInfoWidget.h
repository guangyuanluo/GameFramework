// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class GAMEFRAMEWORKEDITOR_API SGlobalSkillInfoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGlobalSkillInfoWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSubclassOf<class UCoreAbility> CurrentAbility);

private:
	TArray<TSharedPtr<TSubclassOf<class UCoreAbility>> > AbilityData;
	TSharedPtr<SListView< TSharedPtr<TSubclassOf<class UCoreAbility>> >> ListView;

	TSharedRef<ITableRow> SkillInfoListViewOnGenerateRow(TSharedPtr<TSubclassOf<class UCoreAbility>> Item, const TSharedRef<STableViewBase>& OwnerTable);
};