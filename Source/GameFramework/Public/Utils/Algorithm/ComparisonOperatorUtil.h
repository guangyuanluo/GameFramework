// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ComparisonOperatorUtil.generated.h"

/**
 * 比较关系运算符
 */
UENUM(BlueprintType)
enum class ComparisonOperatorEnum :uint8 {
    E_Equal UMETA(DisplayName = "等于"),
	E_NotEqual UMETA(DisplayName = "不等于"),
	E_LessThan UMETA(DisplayName = "小于"),
	E_EqualOrLessThan UMETA(DisplayName = "小于等于"),
	E_GreaterThan UMETA(DisplayName = "大于"),
	E_EqualGreaterThan UMETA(DisplayName = "大于等于"),
};

/**
 * 
 */
UCLASS()
class GAMEFRAMEWORK_API UComparisonOperatorUtil : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

    
	template<typename A, typename B>
    static bool CheckComparePass(const A ValueA, const B ValueB, ComparisonOperatorEnum Operator)
	{
		switch(Operator)
		{
		case ComparisonOperatorEnum::E_Equal:
		return ValueA == ValueB;
		case ComparisonOperatorEnum::E_NotEqual:
		return ValueA != ValueB;
		case ComparisonOperatorEnum::E_LessThan:
		return ValueA < ValueB;
		case ComparisonOperatorEnum::E_EqualOrLessThan:
		return ValueA <= ValueB;
		case ComparisonOperatorEnum::E_GreaterThan:
		return ValueA > ValueB;
		case ComparisonOperatorEnum::E_EqualGreaterThan:
		return ValueA >= ValueB;
		}
		return false;
	}
};