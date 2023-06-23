// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SConditionWidget.h"

class GAMEFRAMEWORKEDITOR_API SConditionWidgetDefault : public SConditionWidget {
public:
	SLATE_BEGIN_ARGS(SConditionWidgetDefault) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, int InChildIndex);

private:
	SVerticalBox::FSlot* ParentSlot;
	SHorizontalBox::FSlot* WidgetSlot;
	int ChildIndex;

	FReply RemoveClick();
	void NotifyConditionChange();
	void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);
	bool GetIsPropertyVisible(const FPropertyAndParent& PropertyAndParent) const;
};