// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreCondition.generated.h"

/**
 * 条件
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API UCoreCondition : public UObject
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* 全局唯一id
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FGuid ID;

	/**
	* 条件进度类型
	*/
	UPROPERTY(transient, BlueprintReadWrite, VisibleAnywhere, Category = "ConditionSystem")
	TSubclassOf<class UCoreConditionProgress> ProgressClass;

	/**
	* 生成guid，如果已经有了，不会再生成
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	void GenerateGuid();

	/**
	* 生成基于该条件的条件进度
	*/
    UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
    class UCoreConditionProgress* GenerateConditionProgress(AActor* ProgressOwner);
};
