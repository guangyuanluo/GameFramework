// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Modules/Condition/ConditionTriggerHandler.h"
#include "Base/ECS/SystemBase.h"
#include "ConditionTriggerSystem.generated.h"

class UCoreConditionProgress;
class UCoreGameInstance;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAllProgressesSatisfyDelegate, FConditionTriggerHandler, Handler);

USTRUCT()
struct FConditionFollowContent {
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UCoreConditionProgress*> Progresses;

	UPROPERTY()
	FConditionTriggerHandler Handler;

	UPROPERTY()
	FOnAllProgressesSatisfyDelegate Callback;
};

/*
* @brief 条件触发系统
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UConditionTriggerSystem : public USystemBase {
public:
	GENERATED_BODY()

	virtual void Initialize(UCoreGameInstance* InGameInstance) override;
	virtual void Uninitialize() override;

	/**
	* @brief 关注条件
	* @param InOutHandler 监听句柄
	* @param progresses 关注的条件进度
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem", meta = (AutoCreateRefTerm = "InDelegate"))
	void FollowConditions(FConditionTriggerHandler& InOutHandler, const TArray<UCoreConditionProgress*>& Progresses, const FOnAllProgressesSatisfyDelegate& InDelegate);

	/**
	* @brief 取消关注条件
	* @param InOutHandler 监听句柄
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	void UnfollowConditions(FConditionTriggerHandler& InOutHandler);

private:
	/** the last serial number we assigned from this condition system */
	static uint64 LastAssignedSerialNumber;
	UPROPERTY()
	TMap<FConditionTriggerHandler, FConditionFollowContent> FollowMap;
	UPROPERTY()
	TMap<UCoreConditionProgress*, FConditionTriggerHandler> ProgressReserveMap;

	/**
	* 生成句柄
	*/
	void GenerateHandler(FConditionTriggerHandler& InOutHandler);

	/**
	* 条件进度满足性变化回调
	*/
	UFUNCTION()
	void OnConditionSatisfyChange(UCoreConditionProgress* Progress, bool NewSatisfy);

	/**
	* 进度是否满足
	*/
	bool IsFollowContentSatisfy(const FConditionFollowContent& FollowContent);
};

/****************Wait Condition begin******************/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSatisfyConditionDelegate);

UCLASS()
class GAMEFRAMEWORK_API UWaitCondition : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCondition* StartWaitCondition(class ACorePlayerController* PlayerController, const TArray<UCoreCondition*>& Conditions);

	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	void Cancel();

	UPROPERTY(BlueprintAssignable)
	FSatisfyConditionDelegate OnSatisfy;
private:
	FConditionTriggerHandler ConditionTriggerHandler;

	UPROPERTY()
	TArray<UCoreConditionProgress*> ConditionProgresses;

	UFUNCTION()
	void OnAllProgressesSatisfy();

};
/****************Wait Condition end******************/