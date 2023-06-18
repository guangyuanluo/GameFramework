// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "PlayerIntimacyRequestCondition.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EIntimacyCompare : uint8
{
	Above 	UMETA(DisplayName = "高于"),
	Below 	UMETA(DisplayName = "低于"),
};
/**
 * 玩家好感度需求条件
 */
UCLASS(BlueprintType, meta = (DisplayName = "玩家好感度需求"))
class GAMEFRAMEWORK_API UPlayerIntimacyRequestCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* npcid
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 NPCId;

	/**
	* 好感度需求
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int32 IntimacyRequest;

	/**
	* 好感度需求方式
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	EIntimacyCompare Compare;
};
