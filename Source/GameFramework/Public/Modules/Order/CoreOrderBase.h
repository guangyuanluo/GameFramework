// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreOrderBase.generated.h"

/**
 * 订单
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API UCoreOrderBase : public UObject
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 全局唯一id
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FGuid ID;

};
