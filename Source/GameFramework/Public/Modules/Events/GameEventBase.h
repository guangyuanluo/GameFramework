// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEventBase.generated.h"

/**
 * 事件基类
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UGameEventBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	class ACorePlayerController* GetOwnerPlayerController() const;
};