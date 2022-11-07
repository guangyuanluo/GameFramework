// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "StoreGlobalEvents.generated.h"

/**
 * @brief 购买商品请求
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UBuyGoodsRequestEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
        class ACoreCharacter* Source;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
        class AActor* Store;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem", Meta = (ExposeOnSpawn = true))
        int32 GoodsID;
};