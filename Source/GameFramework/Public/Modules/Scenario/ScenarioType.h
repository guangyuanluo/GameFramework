// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//需要标记枚举，并设置蓝图类型  
UENUM(BlueprintType)
enum class EScenarioType :uint8 //设置uint8类型  
{
	E_Quest UMETA(DisplayName = "任务剧情"),
	E_Chat UMETA(DisplayName = "闲谈剧情"),
};