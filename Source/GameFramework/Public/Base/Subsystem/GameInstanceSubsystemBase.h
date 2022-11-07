// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameInstanceSubsystemBase.generated.h"

/**
 * GameInstance子系统基类
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UGameInstanceSubsystemBase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	* 初始化
	*/
	UFUNCTION(BlueprintNativeEvent)
	void Init();
};