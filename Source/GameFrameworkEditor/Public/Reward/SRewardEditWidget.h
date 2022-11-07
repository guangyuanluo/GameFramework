// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 奖励编辑界面
 */
class GAMEFRAMEWORKEDITOR_API SRewardEditWidget : public SCompoundWidget
{
public:
	DECLARE_DELEGATE(FOnRewardChange);

	SLATE_BEGIN_ARGS(SRewardEditWidget) {}

		SLATE_EVENT( FOnRewardChange, OnRewardChange )

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UObject* InOuter);

	void RefreshRewardPtr(TArray<class UCoreReward*>* InRewardPtr);

private:
	UObject* Outer;

	TArray<TSharedPtr<FString>> RewardNameSource;
	TSharedPtr<FString> SelectRewardName;
	TSharedPtr<SVerticalBox> RewardPage;
	TArray<class UCoreReward*>* RewardPtr;
	FOnRewardChange OnRewardChange;

	void GenerateRewardWidget();
	TSharedRef<SWidget> GenerateRewardTypeComboItem(TSharedPtr<FString> InItem);
	void RewardNameComboBox_OnSelectionChanged(TSharedPtr<FString> NewGroupingMode, ESelectInfo::Type SelectInfo);
	FReply AddRewardButtonClicked();
	FText GetRewardTypeComboText() const;
	void OnRewardWidgetChange(class UCoreReward* CoreReward);
	void OnRewardWidgetRemove(class UCoreReward* CoreReward);
};