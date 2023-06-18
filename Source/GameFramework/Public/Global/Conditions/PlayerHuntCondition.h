// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "PlayerHuntCondition.generated.h"

/**
 * 玩家boss输出
 */
UCLASS(BlueprintType, meta = (DisplayName = "玩家boss输出"))
class GAMEFRAMEWORK_API UPlayerHuntCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* boss id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int UnitId;

	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	int MasterId;

	/**
	* 输出贡献值
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	float ContributionDegree;
};
