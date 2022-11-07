// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SGameFrameworkMainWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameFrameworkMainWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList);

private:
	TSharedPtr<FUICommandList> CommandList;
	void CheckBaseEditorTableNoExistAndCreate();
};
