// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Modules/Condition/CoreConditionObserver.h"
#include "Modules/Events/EventHandlerInterface.h"
#include "Base/ECS/SystemBase.h"
#include "ConditionSystem.generated.h"

class UCoreConditionProgress;
class UCoreGameInstance;
class UGameEventBase;

UCLASS()
class UFollowContent : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConditionSystem")
	TArray<UCoreConditionProgress*> Progresses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConditionSystem")
	TScriptInterface<ICoreConditionObserver> Observer;
};

UCLASS()
class UProgressReserveInfo : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConditionSystem")
	UFollowContent* ProgressFollowContent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConditionSystem")
	TArray<UClass*> CareEventTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConditionSystem")
	bool LastComplete;
};

USTRUCT(BlueprintType)
struct FProgressEventMapValue {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConditionSystem")
	TArray<UCoreConditionProgress*> Values;
};


/*
* @brief 条件系统
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UConditionSystem : public USystemBase, public IEventHandlerInterface {
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
	TMap<UCoreConditionProgress*, UProgressReserveInfo*> ProgressReserveMap;
	UPROPERTY()
	TMap<UClass*, FProgressEventMapValue> ProgressEventMap;

	/**************EventHandler interface define begin*************/
	virtual TArray<UClass*> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* handleEvent) override;
	/**************EventHandler interface define end*************/
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