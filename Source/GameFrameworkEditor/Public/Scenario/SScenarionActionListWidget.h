// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Slate.h"
#include "Widgets/SCompoundWidget.h"
#include "Delegates/DelegateCombinations.h"


class GAMEFRAMEWORKEDITOR_API SScenarionActionListWidget : public SCompoundWidget
{
public:
	virtual ~SScenarionActionListWidget();

	SLATE_BEGIN_ARGS(SScenarionActionListWidget) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<IPropertyHandle> InActionsProperty);

protected:
	TMap<FString, UClass*> ActionClassMap;
	TArray<TSharedPtr<FString>> ActionClassNameSource;
	TSharedPtr<FString> SelectActionClassName;
	TSharedPtr<IPropertyHandle> ActionsProperty;
	TSharedPtr<SVerticalBox> ActionListPanel;

	TSharedRef<SWidget> GenerateActionComboItem(TSharedPtr<FString> InItem);
	void ActionNameComboBox_OnSelectionChanged(TSharedPtr<FString> NewGroupingMode, ESelectInfo::Type SelectInfo);
	FText GetActionTypeComboText() const;
	FReply AddActionButtonClicked();

	void RefreshActionListPanel();

	void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);
	FReply RemoveClick(int CurrentIndex);

	void OnMarkDirty();
};