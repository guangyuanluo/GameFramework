// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Order/CoreOrderBase.h"
#include "Modules/Item/ItemIDNumPair.h"
#include "ItemOrder.generated.h"

/**
 * 物品订单
 */
UCLASS()
class GAMEFRAMEWORK_API UItemOrder : public UCoreOrderBase
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 订单物品
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FItemIDNumPair> Items;
};
