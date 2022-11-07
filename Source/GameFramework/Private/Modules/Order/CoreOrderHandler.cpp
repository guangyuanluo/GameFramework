// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreOrderHandler.h"

class UCoreOrderBase* UCoreOrderHandler::OnCreateOrder_Implementation(UCoreGameInstance* InGameInstance, TSubclassOf<class UCoreOrderBase> OrderClass, UObject* Context) {
    return nullptr;
}

bool UCoreOrderHandler::OnPayOrder_Implementation(UCoreGameInstance* InGameInstance, class ACoreCharacter* Source, class UCoreOrderBase* Order) {
    return false;
}