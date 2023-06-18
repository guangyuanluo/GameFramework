// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 条件编辑界面
 */
class GAMEFRAMEWORKEDITOR_API SConditionEditWidget : public SCompoundWidget
{
public:
	DECLARE_DELEGATE(FOnConditionChange);

	SLATE_BEGIN_ARGS(SConditionEditWidget) {}

		SLATE_EVENT( FOnConditionChange, OnConditionChange )

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UObject* InOuter);

	void RefreshConditionPtr(TArray<class UCoreCondition*>* InConditionPtr);

private:
	TArray<TSharedPtr<FString>> ConditionNameSource;
	TMap<FString, TSubclassOf<class UCoreCondition>> ConditionNameMap;
	TSharedPtr<FString> SelectConditionName;
	TSharedPtr<SVerticalBox> ConditionPage;
	TArray<class UCoreCondition*>* ConditionPtr;
	FOnConditionChange OnConditionChange;
	UObject* Outer;

	void GenerateConditionWidget();
	TSharedRef<SWidget> GenerateConditionTypeComboItem(TSharedPtr<FString> InItem);
	void ConditionNameComboBox_OnSelectionChanged(TSharedPtr<FString> NewGroupingMode, ESelectInfo::Type SelectInfo);
	FReply AddConditionButtonClicked();
	FText GetConditionTypeComboText() const;
	void OnConditionWidgetChange(class UCoreCondition* CoreCondition);
	void OnConditionWidgetRemove(class UCoreCondition* CoreCondition);
};