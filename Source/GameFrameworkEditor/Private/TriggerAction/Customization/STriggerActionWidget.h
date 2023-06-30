// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Slate.h"
#include "Widgets/SCompoundWidget.h"
#include "Delegates/DelegateCombinations.h"

class STriggerActionWidget : public SCompoundWidget {
public:
	DECLARE_DELEGATE_OneParam(FOnTriggerActionWidgetChange, class UCoreTriggerAction*);
	DECLARE_DELEGATE_OneParam(FOnTriggerActionWidgetPreremove, class UCoreTriggerAction*);

	SLATE_BEGIN_ARGS(STriggerActionWidget) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UCoreTriggerAction* InWidgetAction, SVerticalBox::FSlot* InParentSlot);

	FOnTriggerActionWidgetChange OnActionWidgetChange;
	FOnTriggerActionWidgetPreremove OnActionWidgetPreremove;

private:
	SVerticalBox::FSlot* ParentSlot;
	SHorizontalBox::FSlot* WidgetSlot;
	class UCoreTriggerAction* WidgetAction;

	FReply RemoveClick();
	void NotifyActionChange();
	void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);
};