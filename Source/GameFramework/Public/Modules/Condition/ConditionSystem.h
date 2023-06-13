// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Modules/Condition/CoreConditionObserver.h"
#include "Base/ECS/SystemBase.h"
#include "ConditionSystem.generated.h"

class UCoreConditionProgress;
class UCoreGameInstance;
class UGameEventBase;

UCLASS()
class UFollowContent : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UCoreConditionProgress*> Progresses;

	UPROPERTY()
	TScriptInterface<ICoreConditionObserver> Observer;
};

USTRUCT()
struct FProgressReserveInfo {
	GENERATED_BODY()

public:
	UPROPERTY()
	UFollowContent* ProgressFollowContent;

	UPROPERTY()
	bool LastComplete;
};

/*
* @brief 条件系统
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UConditionSystem : public USystemBase {
public:
	GENERATED_BODY()

	virtual void Initialize(UCoreGameInstance* InGameInstance) override;
	virtual void Uninitialize() override;

	/**
	* @brief 关注条件
	* @param progresses 关注的条件进度
	* @param observer 关注的条件观察者
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	void FollowConditions(const TArray<UCoreConditionProgress*>& Progresses, TScriptInterface<ICoreConditionObserver> Observer);

	/**
	* @brief 取消关注条件
	* @param observer 关注的条件观察者
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	void UnfollowConditions(TScriptInterface<ICoreConditionObserver> Observer);

	/**
	* @brief 从进度反向获取到观察者
	*/
	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	TScriptInterface<ICoreConditionObserver> GetObserverFromProgress(UCoreConditionProgress* InProgress);

private:
	UPROPERTY()
	TMap<UObject*, UFollowContent*> FollowMap;
	UPROPERTY()
	TMap<UCoreConditionProgress*, FProgressReserveInfo> ProgressReserveMap;

	/**
	* 条件进度满足性变化回调
	*/
	UFUNCTION()
	void OnConditionSatisfyChange(UCoreConditionProgress* Progress, bool NewSatisfy);

	/**
	* 进度是否满足
	*/
	bool IsFollowContentSatisfy(UFollowContent* FollowContentObj);
};

/****************Wait Condition begin******************/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSatisfyConditionDelegate);

UCLASS()
class GAMEFRAMEWORK_API UWaitCondition : public UBlueprintAsyncActionBase, public ICoreConditionObserver
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCondition* StartWaitCondition(UWaitCondition* WaitCondition);

	UFUNCTION(BlueprintCallable, Category = "ConditionSystem")
	void Cancel();

	UPROPERTY(BlueprintAssignable)
	FSatisfyConditionDelegate OnSatisfy;

	//ICoreConditionObserver implement begine
	virtual void OnSatisfyConditions_Implementation(const TArray<UCoreConditionProgress*>& Progresses) override;
	virtual void OnProgressRefresh_Implementation(UCoreConditionProgress* ChangeProgress) override;

	UPROPERTY()
	TArray<UCoreConditionProgress*> ConditionProgress;
};
/****************Wait Condition end******************/