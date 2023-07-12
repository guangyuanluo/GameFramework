// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/Subsystem/GameWorldSubsystem.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilityCounterSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCounterNumChange, const FString&, CounterName, int, CounterNum);

USTRUCT()
struct FCounterInfo {
    GENERATED_BODY()

    UPROPERTY()
    int Num;

    UPROPERTY()
    class UCoreAbility* OwnerAbility;
};

/**
* 技能计数器子系统
*/
UCLASS()
class GAMEFRAMEWORK_API UAbilityCounterSubsystem : public UGameWorldSubsystem {
    GENERATED_BODY()

public:
    /**
    * 设置计数器计数
    */
    UFUNCTION(BlueprintCallable)
    void SetCounterNum(const FString& CounterName, int CounterNum, class UCoreAbility* OwnerAbility);

    /**
    * 更改计数器计数
    */
    UFUNCTION(BlueprintCallable)
    void ChangeCounterNum(const FString& CounterName, int DiffNum);

    /**
    * 销毁计数器
    */
    UFUNCTION(BlueprintCallable)
    void DestroyCounter(const FString& CounterName);

    /**
    * 获取计数器数量，不存在返回0
    */
    UFUNCTION(BlueprintPure)
    int GetCounterNum(const FString& CounterName) const;

    /**
    * 计数器变化回调
    */
    UPROPERTY(BlueprintAssignable)
    FOnAbilityCounterNumChange OnAbilityCounterNumChange;

private:
    UPROPERTY()
    TMap<FString, FCounterInfo> CounterMap;

    UFUNCTION()
    void OnAbilityEnd(const FAbilityEndedData& AbilityEndedData, FString CounterName);
};