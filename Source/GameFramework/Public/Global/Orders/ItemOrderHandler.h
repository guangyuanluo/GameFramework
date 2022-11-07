// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Order/CoreOrderHandler.h"
#include "ItemOrderHandler.generated.h"

/*
* @brief 物品订单处理
*/
UCLASS()
class GAMEFRAMEWORK_API UItemOrderHandler : public UCoreOrderHandler {
    GENERATED_BODY()

public:
    virtual class UCoreOrderBase* OnCreateOrder_Implementation(UCoreGameInstance* InGameInstance, TSubclassOf<class UCoreOrderBase> OrderClass, UObject* Context) override;
    virtual bool OnPayOrder_Implementation(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source, class UCoreOrderBase* Order) override;
};