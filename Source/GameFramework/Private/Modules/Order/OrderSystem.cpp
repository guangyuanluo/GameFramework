// Fill out your copyright notice in the Description page of Project Settings.

#include "OrderSystem.h"
#include "UE4LogImpl.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "EventSystem.h"
#include "GameEventUtils.h"
#include "OrderSetting.h"
#include "CoreOrderHandler.h"
#include "CoreOrderBase.h"
#include "OrderCancelEvent.h"
#include "OrderCompleteEvent.h"
#include "OrderGlobalEvents.h"

void UOrderSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);

    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->RegistEventHandler(this);
}

void UOrderSystem::Uninitialize() {
	Super::Uninitialize();
}

UCoreOrderBase* UOrderSystem::GenerateNewOrder(TSubclassOf<UCoreOrderBase> OrderClass, UObject* Context) {
    const UOrderSetting* OrderSetting = GetDefault<UOrderSetting>();
    auto FindHandler = OrderSetting->OrderHandlerClass.Find(OrderClass);
    if (!FindHandler) {
        UE_LOG(GameCore, Warning, TEXT("找不对指定订单类的处理类"));
        return nullptr;
    }
    auto HandlerDefaultObject = Cast<UCoreOrderHandler>(FindHandler->GetDefaultObject());
    if (!HandlerDefaultObject) {
        UE_LOG(GameCore, Warning, TEXT("找不对指定订单处理类的默认对象"));
        return nullptr;
    }
    auto NewOrder = HandlerDefaultObject->OnCreateOrder(GameInstance, OrderClass, Context);
    if (!NewOrder) {
        UE_LOG(GameCore, Warning, TEXT("生成订单失败"));
        return nullptr;
    }
    check(!AllOrders.Contains(NewOrder->ID));
    AllOrders.Add(NewOrder->ID, NewOrder);
    return NewOrder;
}

void UOrderSystem::PayOrder(class ACoreCharacter* Character, const FGuid& OrderID) {
    UE_LOG(GameCore, Log, TEXT("尝试支付订单:%s"), *OrderID.ToString());
    auto FindOrderPtr = AllOrders.Find(OrderID);
    if (FindOrderPtr) {
        const UOrderSetting* OrderSetting = GetDefault<UOrderSetting>();
        auto FindHandler = OrderSetting->OrderHandlerClass.Find((*FindOrderPtr)->GetClass());
        check(FindHandler);
        auto HandlerDefaultObject = Cast<UCoreOrderHandler>(FindHandler->GetDefaultObject());
        check(HandlerDefaultObject);
        if (!HandlerDefaultObject->OnPayOrder(GameInstance, Character, *FindOrderPtr)) {
            UE_LOG(GameCore, Warning, TEXT("订单支付失败:%s"), *OrderID.ToString());
            return;
        }
        UE_LOG(GameCore, Warning, TEXT("订单完成:%s"), *OrderID.ToString());
        AllOrders.Remove(OrderID);
        UOrderCompleteEvent* OrderCompleteEvent = NewObject<UOrderCompleteEvent>();
        OrderCompleteEvent->OrderID = OrderID;
        UGameEventUtils::PushEvent(GameInstance, OrderCompleteEvent);
    }
}

void UOrderSystem::CancelOrder(const FGuid& OrderID) {
    UCoreOrderBase* CancelOrder;
    if (AllOrders.RemoveAndCopyValue(OrderID, CancelOrder)) {
        UE_LOG(GameCore, Log, TEXT("订单取消:%s"), *OrderID.ToString());
        UOrderCancelEvent* OrderCancelEvent = NewObject<UOrderCancelEvent>();
        OrderCancelEvent->OrderID = OrderID;
        UGameEventUtils::PushEvent(GameInstance, OrderCancelEvent);
    }
}

TArray<TSubclassOf<class UGameEventBase>> UOrderSystem::GetHandleEventTypes() {
    return {
        UPayOrderRequestEvent::StaticClass(),
    };
}

void UOrderSystem::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
    if (HandleEvent->IsA(UPayOrderRequestEvent::StaticClass())) {
        auto PayOrderRequestEvent = Cast<UPayOrderRequestEvent>(HandleEvent);
        PayOrder(PayOrderRequestEvent->Source, PayOrderRequestEvent->OrderID);
    }
}