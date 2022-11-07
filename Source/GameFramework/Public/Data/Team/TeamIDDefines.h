// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//需要标记枚举，并设置蓝图类型  
UENUM(BlueprintType)
enum class TeamIdDefines :uint8 //设置uint8类型  
{
	E_Team1 UMETA(DisplayName = "队伍1"),
	E_Team2 UMETA(DisplayName = "队伍2"),
	E_Team3 UMETA(DisplayName = "队伍3"),
	E_Team4 UMETA(DisplayName = "队伍4"),
	E_Team5 UMETA(DisplayName = "队伍5"),
	E_Team6 UMETA(DisplayName = "队伍6"),
	E_Team7 UMETA(DisplayName = "队伍7"),
	E_Team8 UMETA(DisplayName = "队伍8"),
	E_Team9 UMETA(DisplayName = "队伍9"),
	E_Team10 UMETA(DisplayName = "队伍10"),
	E_Team11 UMETA(DisplayName = "队伍11"),
	E_Team12 UMETA(DisplayName = "队伍12"),
	E_Team13 UMETA(DisplayName = "队伍13"),
	E_Team14 UMETA(DisplayName = "队伍14"),
	E_Team15 UMETA(DisplayName = "中立友好"),
	E_Team16 UMETA(DisplayName = "中立敌对"),
};