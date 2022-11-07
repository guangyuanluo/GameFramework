// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "OrderCancelEvent.generated.h"

/**
 * @brief 订单取消 event
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UOrderCancelEvent : public UGameEventBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
	FGuid OrderID;
};