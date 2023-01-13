// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreOrderHandler.generated.h"

class UCoreGameInstance;

/*
* @brief 订单处理
*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UCoreOrderHandler : public UObject {
    GENERATED_BODY()

public:
    /**
    * 生成订单对象
    */
    UFUNCTION(BlueprintNativeEvent)
    class UCoreOrderBase* OnCreateOrder(UCoreGameInstance* InGameInstance, TSubclassOf<class UCoreOrderBase> OrderClass, UObject* Context);

    /**
    * 订单支付
    */
    UFUNCTION(BlueprintNativeEvent)
    bool OnPayOrder(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source, class UCoreOrderBase* Order);
};