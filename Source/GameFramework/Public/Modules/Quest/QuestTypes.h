// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.generated.h"

//任务类型，预留多个，可以在EnumDisplayNameConfigSetting设置DisplayName
UENUM(BlueprintType, meta = (DisplayName = "任务类型"))
enum class EQuestTypeEnum :uint8 //设置uint8类型  
{
	QuestType_1,
	QuestType_2,
	QuestType_3,
	QuestType_4,
	QuestType_5,
	QuestType_6,
	QuestType_7,
	QuestType_8,
	QuestType_9,
};