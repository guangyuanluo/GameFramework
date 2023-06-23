// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Slate.h"
#include "Widgets/SCompoundWidget.h"
#include "Delegates/DelegateCombinations.h"

class UCoreCondition;


class GAMEFRAMEWORKEDITOR_API SConditionWidget : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnConditionWidgetChange, UCoreCondition*);
	DECLARE_DELEGATE_OneParam(FOnConditionWidgetPreremove, UCoreCondition*);

	virtual ~SConditionWidget();

	SLATE_BEGIN_ARGS(SConditionWidget) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, int ChildIndex);

	UCoreCondition* GetWidgetCondition() { return WidgetCondition; }

	FOnConditionWidgetChange OnConditionWidgetChange;
	FOnConditionWidgetPreremove OnConditionWidgetPreremove;

protected:
	UCoreCondition* WidgetCondition;
};