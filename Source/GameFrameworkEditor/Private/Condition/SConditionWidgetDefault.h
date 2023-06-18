// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SConditionWidget.h"

class SConditionWidgetDefault : public SConditionWidget {
public:
	SLATE_BEGIN_ARGS(SConditionWidgetDefault) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot);

private:
	SVerticalBox::FSlot* ParentSlot;
	SHorizontalBox::FSlot* WidgetSlot;

	FReply RemoveClick();
	void NotifyConditionChange();
	void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);
};