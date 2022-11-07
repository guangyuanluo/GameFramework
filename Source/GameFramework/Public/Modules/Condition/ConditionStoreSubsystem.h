// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/Subsystem/GameInstanceSubsystemBase.h"
#include "Modules/Condition/CoreCondition.h"
#include "ConditionStoreSubsystem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UConditionStoreSubsystem : public UGameInstanceSubsystemBase
{
public:
	GENERATED_BODY()

	/**
	* 注册condition
	*/
	UFUNCTION(BlueprintCallable, Category = "Condition")
	void RegistCondition(UCoreCondition* InCondition);

	/**
	* 反注册condition
	*/
	UFUNCTION(BlueprintCallable, Category = "Condition")
	void UnregistCondition(UCoreCondition* InCondition);

	/**
	* 通过guid获取condition
	*/
	UFUNCTION(BlueprintPure, Category = "Condition")
	UCoreCondition* GetCondition(const FGuid& InGuid);

private:
	UPROPERTY()
	TMap<FGuid, UCoreCondition*> ConditionMap;
};