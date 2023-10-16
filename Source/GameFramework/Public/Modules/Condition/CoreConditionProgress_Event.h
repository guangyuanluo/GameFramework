// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "Modules/Events/EventHandlerInterface.h"
#include "CoreConditionProgress_Event.generated.h"

/**
 * 基于事件系统的条件进度
 */
UCLASS(Abstract)
class GAMEFRAMEWORK_API UCoreConditionProgress_Event : public UCoreConditionProgress, public IEventHandlerInterface
{
public:
	GENERATED_BODY()

	virtual void OnInitialize() override;
	virtual void OnUninitialize() override;
	virtual void OnSatisfyChange() override;
};
