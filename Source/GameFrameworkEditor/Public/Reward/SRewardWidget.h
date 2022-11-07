// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Slate.h"
#include "Widgets/SCompoundWidget.h"
#include "Delegates/DelegateCombinations.h"

class UCoreReward;


class GAMEFRAMEWORKEDITOR_API SRewardWidget : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnRewardWidgetChange, UCoreReward*);
	DECLARE_DELEGATE_OneParam(FOnRewardWidgetPreremove, UCoreReward*);

	virtual ~SRewardWidget();

	SLATE_BEGIN_ARGS(SRewardWidget) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UCoreReward* InWidgetReward);

	UCoreReward* GetWidgetReward() { return WidgetReward; }

	FOnRewardWidgetChange OnRewardWidgetChange;
	FOnRewardWidgetPreremove OnRewardWidgetPreremove;

protected:
	UCoreReward* WidgetReward;
};