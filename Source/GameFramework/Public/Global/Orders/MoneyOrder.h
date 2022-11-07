// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Order/CoreOrderBase.h"
#include "Modules/Money/MoneyTypeNumPair.h"
#include "MoneyOrder.generated.h"

/**
 * 货币订单
 */
UCLASS()
class GAMEFRAMEWORK_API UMoneyOrder : public UCoreOrderBase {
public:
	GENERATED_UCLASS_BODY()

	/**
	* 订单货币
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FMoneyTypeNumPair> Money;
};
