// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/SystemBase.h"
#include "Modules/Events/EventHandlerInterface.h"
#include "OrderSystem.generated.h"

class UCoreGameInstance;
class UCoreOrderBase;

/*
* @brief 订单系统
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UOrderSystem : public USystemBase, public IEventHandlerInterface {
public:
	GENERATED_BODY()

	virtual void Initialize(UCoreGameInstance* InGameInstance) override;
	virtual void Uninitialize() override;

	/**
	* 生成新订单
	*/
	UCoreOrderBase* GenerateNewOrder(TSubclassOf<UCoreOrderBase> OrderClass, UObject* Context);

	/**
	* 支付订单
	*/
	void PayOrder(class ACoreCharacter* Character, const FGuid& OrderID);

	/**
	* 取消订单
	*/
	void CancelOrder(const FGuid& OrderID);

private:
	/**
	* 所有订单
	*/
	UPROPERTY()
	TMap<FGuid, UCoreOrderBase*> AllOrders;

	/** 覆写事件监听 */
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
};