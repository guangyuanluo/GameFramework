// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "OrderSetting.generated.h"

/*
* @brief 订单设置
*/
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "订单设置"))
class GAMEFRAMEWORK_API UOrderSetting : public UDeveloperSettings {
    GENERATED_BODY()

public:
    UPROPERTY(config, EditAnywhere, Category = "Order", meta = (DisplayName = "订单处理类", AllowAbstract = "false"))
    TMap<TSubclassOf<class UCoreOrderBase>, TSubclassOf<class UCoreOrderHandler>> OrderHandlerClass;

    virtual FName GetCategoryName() const override;
};