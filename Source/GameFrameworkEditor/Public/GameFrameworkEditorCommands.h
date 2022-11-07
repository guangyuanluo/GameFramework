// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "GameFrameworkEditorStyle.h"

class FGameFrameworkEditorCommands : public TCommands<FGameFrameworkEditorCommands>
{
public:

	FGameFrameworkEditorCommands()
		: TCommands<FGameFrameworkEditorCommands>(TEXT("GameFrameworkEditor"), NSLOCTEXT("Contexts", "GameFrameworkEditor", "GameFrameworkEditor Plugin"), NAME_None, FGameFrameworkEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
	//任务界面右键菜单
	TSharedPtr< FUICommandInfo > AddNewTopQuestNode;			//添加顶层任务节点
	TSharedPtr< FUICommandInfo > AddNewChildQuestNode;			//添加子任务节点
	TSharedPtr< FUICommandInfo > DeleteQuestNodeAll;			//删除任务节点及所有后续任务
	TSharedPtr< FUICommandInfo > DeleteQuestNodeOnly;			//仅删除当前任务节点
};