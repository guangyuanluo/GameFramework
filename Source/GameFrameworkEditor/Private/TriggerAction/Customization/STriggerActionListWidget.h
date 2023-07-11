// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 触发动作编辑界面
 */
class STriggerActionListWidget : public SCompoundWidget
{
public:
	DECLARE_DELEGATE(FOnActionChange);

	SLATE_BEGIN_ARGS(STriggerActionListWidget) {}

		SLATE_EVENT( FOnActionChange, OnActionChange )

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UObject* InOuter);

	void RefreshActionArrayPtr(TArray<class UCoreTriggerAction*>* InActionArrayPtr);

private:
	UClass* SelectActionClass;
	TSharedPtr<SVerticalBox> TriggerActionPage;
	TArray<class UCoreTriggerAction*>* ActionArrayPtr;
	FOnActionChange OnActionChange;
	UObject* Outer;

	void GenerateActionWidget();
	FReply AddActionButtonClicked();
	FReply CopyButtonClicked();
	FReply PasteButtonClicked();
	FReply ClearButtonClicked();
	void OnActionWidgetChange(class UCoreTriggerAction* TriggerAction);
	void OnActionWidgetRemove(class UCoreTriggerAction* TriggerAction);
};