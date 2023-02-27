// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimpleCodePipeline.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FPipelineSyncDynFunction, UObject*, Context);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(class UAdvanceObjectPromise*, FPipelineAsyncDynFunction, UObject*, Context);

DECLARE_DELEGATE_OneParam(FPipelineSyncFunction, UObject*);
DECLARE_DELEGATE_RetVal_OneParam(class UAdvanceObjectPromise*, FPipelineAsyncFunction, UObject*);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCodePipelineProgressChange, class USimpleCodePipeline*, Pipeline, int, CurrentIndex, int, TotalIndex);

/**
* 简单代码执行管线
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API USimpleCodePipeline : public UObject {
	GENERATED_UCLASS_BODY()

public:
	/*
	* 管线debug名字
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FString DebugPipelineName;

	/**
	 * 管线上下文
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	UObject* Context;

	/**
	 * 是否显示调试信息
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool ShowDebugInfo = false;

	/**
	* 往管线里面加同步执行代码，先加先执行
	* SyncFunction 同步函数
	* Condition 类似语法糖，减少蓝图各种if连接，如果为false等于这个push不生效
	* IsFraming 是否分帧，某些函数基于性能考虑，可能需要下帧执行，减少本帧占用时间
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PushSyncFunction", ScriptName = "PushSyncFunction", AutoCreateRefTerm = "SyncFunction"))
	USimpleCodePipeline* K2_PushSyncFunction(const FPipelineSyncDynFunction& SyncFunction, bool Condition = true, bool IsFraming = false, const FString& DebugName = TEXT(""));

	USimpleCodePipeline* PushSyncFunction(const FPipelineSyncFunction& SyncFunction, bool IsFraming = false, const FString& DebugName = TEXT(""));

	/**
	* 往管线里面加异步执行代码，先加先执行
	* AsyncFunction 异步函数，基于promise判断是否完成
	* Condition 类似语法糖，减少蓝图各种if连接，如果为false等于这个push不生效
	* IsFraming 是否分帧，某些函数基于性能考虑，可能需要下帧执行，减少本帧占用时间
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PushAsyncFunction", ScriptName = "PushAsyncFunction", AutoCreateRefTerm = "AsyncFunction"))
	USimpleCodePipeline* K2_PushAsyncFunction(const FPipelineAsyncDynFunction& AsyncFunction, bool Condition = true, bool IsFraming = false, const FString& DebugName = TEXT(""));

	USimpleCodePipeline* PushAsyncFunction(const FPipelineAsyncFunction& AsyncFunction, bool IsFraming = false, const FString& DebugName = TEXT(""));

	/**
	* 开始执行管线
	*/
	UFUNCTION(BlueprintCallable)
	class UAdvanceObjectPromise* Execute();

	/**
	* 取消管线执行
	*/
	UFUNCTION(BlueprintCallable)
	void Cancel();

	/**
	* 进度回调
	*/
	UPROPERTY(BlueprintAssignable)
	FOnCodePipelineProgressChange OnCodePipelineProgressChange;

private:
	UPROPERTY()
	class UAdvanceObjectPromise* ExecutePromise;

	struct FPipelineFunction {
		bool IsAsyncFunction;
		bool IsFraming;
		
		FPipelineSyncFunction SyncFunction;
		FPipelineAsyncFunction AsyncFunction;

		FString DebugName;
	};

	int CurrentExecuteIndex = 0;
	TArray<FPipelineFunction> Executors;

	void RunExecutor();
	void ExecuteNext();

	UFUNCTION()
	void ExecutorCallbackSuccess(UObject* Result);

	UFUNCTION()
	void ExecutorCallbackFail(const FString& FailureReason);

	void ExecutorExecuteComplete();

	void SetReadyToDestroy();
};