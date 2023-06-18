// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreCondition.h"
#include "CoreConditionProgress.generated.h"

class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConditionProgressSatisfyUpdate, UCoreConditionProgress*, Progress, bool, NewSatisfy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConditionProgressPostNetReceive, UCoreConditionProgress*, Progress);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UCoreConditionProgress : public UObject
{
public:
	GENERATED_BODY()

	/**
	* 条件
	*/
	UPROPERTY(ReplicatedUsing = OnRep_Condition, BlueprintReadOnly, VisibleAnywhere, Category = "ConditionSystem")
	UCoreCondition* Condition;

	/**
	* 拥有者
	*/
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "ConditionSystem")
	AActor* ProgressOwner;

	/**
	* 进度满足性更新回调，从满足变成不满足，从不满足变成满足时触发
	*/
	UPROPERTY(BlueprintAssignable)
	FOnConditionProgressSatisfyUpdate OnConditionProgressSatisfyUpdate;

	/**
	* 进度被网络同步的时候的回调
	*/
	UPROPERTY(BlueprintAssignable)
	FOnConditionProgressPostNetReceive OnConditionProgressPostNetReceive;

	/**
	* 初始化完成
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ConditionSystem")
	void PostProgressInitialize();

	/**
	* 开始监听条件
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ConditionSystem")
	void StartFollow();

	/**
	* 停止监听条件
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ConditionSystem")
	void StopFollow();

	/*
	* @brief 是否完成
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConditionSystem")
	bool IsComplete();

	/**
	* @brief 条件完成后的处理
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ConditionSystem")
	void HandleComplete();

	/**
	* 刷新满足性
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	void RefreshSatisfy();

	/**
	* 满足性变更
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ConditionSystem")
	void OnSatisfyChange();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override;
	virtual void PostNetReceive() override;

private:
	UPROPERTY(Transient)
	bool LastSatisfy = false;

	UFUNCTION()
	void OnRep_Condition();
};
