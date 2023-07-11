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
	UClass* SelectConditionClass;
	TSharedPtr<SVerticalBox> ConditionPage;
	TArray<class UCoreCondition*>* ConditionPtr;
	FOnConditionChange OnConditionChange;
	UObject* Outer;

	void GenerateConditionWidget();
	FReply AddConditionButtonClicked();
	FReply CopyButtonClicked();
	FReply PasteButtonClicked();
	FReply ClearButtonClicked();
	void OnConditionWidgetChange(class UCoreCondition* CoreCondition);
	void OnConditionWidgetRemove(class UCoreCondition* CoreCondition);
};