// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GameFrameworkEditorCommands.h"

#define LOCTEXT_NAMESPACE "FGameFrameworkEditorModule"

void FGameFrameworkEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "编辑", "对游戏的任务、物品等进行编辑", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(AddNewTopQuestNode, "新增任务", "添加一个新的顶层任务模板进行编辑，该任务模板没有需要前置完成的任务", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(AddNewChildQuestNode, "添加后续任务", "添加一个任务，新增任务会以当前选中任务为前提任务", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(DeleteQuestNodeAll, "删除任务", "删除当前任务及该任务后续的所有任务", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(DeleteQuestNodeOnly, "仅删除当前任务", "仅删除当前任务，该任务后续的所有任务会移到前置任务中", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
