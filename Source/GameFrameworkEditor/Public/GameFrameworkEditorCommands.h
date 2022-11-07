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
	//��������Ҽ��˵�
	TSharedPtr< FUICommandInfo > AddNewTopQuestNode;			//��Ӷ�������ڵ�
	TSharedPtr< FUICommandInfo > AddNewChildQuestNode;			//���������ڵ�
	TSharedPtr< FUICommandInfo > DeleteQuestNodeAll;			//ɾ������ڵ㼰���к�������
	TSharedPtr< FUICommandInfo > DeleteQuestNodeOnly;			//��ɾ����ǰ����ڵ�
};