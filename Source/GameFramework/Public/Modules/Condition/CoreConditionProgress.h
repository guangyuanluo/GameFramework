// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
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
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "ConditionSystem")
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
	* 初始化
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	virtual void Initialize();

	/**
	* 反初始化
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	virtual void Uninitialize();
	/**
	* 是否完成初始化
	*/
	UFUNCTION(BlueprintPure, Category = "ConditionSystem")
	bool IsInitialized() const;

	/*
	* 是否完成
	* @param IsValid 是否判断合法
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "ConditionSystem")
	bool K2_IsComplete(bool& IsValid);

	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	virtual bool IsComplete(bool& IsValid);

	/**
	* 刷新满足性
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	void RefreshSatisfy();

	/**
	* 满足性变更
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "ConditionSystem", DisplayName = "OnSatisfyChange", meta = (ScriptName = "OnSatisfyChange"))
	void K2_OnSatisfyChange();

	virtual void OnSatisfyChange();

	/**
	* 获取进度对象，带孩子
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	virtual void GetProgressesWithChildren(TArray<UCoreConditionProgress*>& OutProgresses);

	/**
    * 获取条件关联的ASC
    */
    UFUNCTION(BlueprintPure)
    class UCoreAbilitySystemComponent* GetAbilitySystemComponent() const;

	virtual UWorld* GetWorld() const override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override;
	virtual void PostNetReceive() override;

protected:
	/**
	* 初始化覆写
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "ConditionSystem", DisplayName = "OnInitialize", meta = (ScriptName = "OnInitialize"))
	void K2_OnInitialize();

	virtual void OnInitialize();


	/**
	* 反初始化覆写
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "ConditionSystem", DisplayName = "OnUninitialize", meta = (ScriptName = "OnUninitialize"))
	void K2_OnUninitialize();

	virtual void OnUninitialize();

private:
	bool bInitialized = false;

	UPROPERTY(Transient)
	bool bLastSatisfy = false;
};
