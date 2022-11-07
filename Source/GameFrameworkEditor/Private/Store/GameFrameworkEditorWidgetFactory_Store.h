// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFrameworkEditorWidgetFactory.h"

/**
 * 
 */
class GameFrameworkEditorWidgetFactory_Store : public GameFrameworkEditorWidgetFactory
{
public:
	virtual FText GetDisplayName() override;
	virtual FText GetDisplayToolTip() override;
	virtual FText GetWindowName() override;
	virtual TSharedRef<SWidget> ConstructPage(TSharedPtr<FUICommandList> CommandList) override;
	virtual void CheckEditorTableNoExistAndCreate() override;
	virtual void Export() override;
    virtual bool CheckOpenCondition() override;
};