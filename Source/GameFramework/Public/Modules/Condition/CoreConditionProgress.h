// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreCondition.h"
#include "CoreConditionProgress.generated.h"

class ACharacter;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API UCoreConditionProgress : public UObject
{
public:
	GENERATED_BODY()

	/**
	* 条件
	*/
	UPROPERTY(Transient, BlueprintReadOnly, VisibleAnywhere, Category = "ConditionSystem")
	UCoreCondition* Condition;

    /**
    * 条件id
    */
    UPROPERTY(ReplicatedUsing = OnRep_ConditionID, BlueprintReadOnly, VisibleAnywhere, Category = "ConditionSystem")
    FGuid ConditionID;
	/**
	* 拥有者
	*/
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "ConditionSystem")
	AActor* ProgressOwner;

	/**
	* 初始化完成
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ConditionSystem")
	void PostProgressInitialize();

	/**
	* 得到关心的事件类型
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ConditionSystem")
	TArray<TSubclassOf<class UGameEventBase>> GetCareEventTypes();

	/**
	* 处理事件，返回值是是否处理了事件
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "ConditionSystem")
	bool ProgressGameEvent(UGameEventBase* GameEvent);

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

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override;

private:
    UFUNCTION()
    void OnRep_ConditionID();
};
