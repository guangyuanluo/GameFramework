// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Framework/Commands/UICommandList.h"
/**
 * 
 */
class GAMEFRAMEWORKEDITOR_API GameFrameworkEditorWidgetFactory
{
public:
	virtual FText GetDisplayName() = 0;
	virtual FText GetDisplayToolTip() = 0;
	virtual FText GetWindowName() = 0;
	virtual TSharedRef<SWidget> ConstructPage(TSharedPtr<FUICommandList> CommandList) = 0;
	virtual void CheckEditorTableNoExistAndCreate() = 0;
	virtual void Export() = 0;
    virtual bool CheckOpenCondition();
	virtual void Open(TSharedPtr<FUICommandList> InCommandList);
};