// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Algorithm/BooleanAlgebraTypes.h"
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
	* 布尔运算关系符
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	BooleanAlgebraEnum Relation = BooleanAlgebraEnum::E_AND;

	/**
	* 是否取反
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	bool bNot = false;

	/**
	* 条件进度类型
	*/
	UPROPERTY(transient, BlueprintReadWrite, VisibleAnywhere, Category = "ConditionSystem")
	TSubclassOf<class UCoreConditionProgress> ProgressClass;

	/**
	* 生成基于该条件的条件进度
	*/
    UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
    class UCoreConditionProgress* GenerateConditionProgress(AActor* ProgressOwner);
};
