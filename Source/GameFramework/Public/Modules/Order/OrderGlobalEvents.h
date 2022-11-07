// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "OrderGlobalEvents.generated.h"

/**
 * @brief 支付订单请求
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPayOrderRequestEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
        class ACoreCharacter* Source;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
        FGuid OrderID;
};