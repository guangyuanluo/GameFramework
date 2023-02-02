// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleCodePipeline.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FPipelineSyncFunction, UObject*, Context);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(class UAdvanceObjectPromise*, FPipelineAsyncFunction, UObject*, Context);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCodePipelineProgressChange, class USimpleCodePipeline*, Pipeline, int, CurrentIndex, int, TotalIndex);

/**
* 简单代码执行管线
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API USimpleCodePipeline : public UObject {
	GENERATED_BODY()

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
	* 往管线里面加同步执行代码，先加先执行
	* SyncFunction 同步函数
	* IsFraming 是否分帧，某些函数基于性能考虑，可能需要下帧执行，减少本帧占用时间
	*/
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "SyncFunction"))
	USimpleCodePipeline* PushSyncFunction(const FPipelineSyncFunction& SyncFunction, bool IsFraming = false);

	/**
	* 往管线里面加异步执行代码，先加先执行
	* AsyncFunction 异步函数，基于promise判断是否完成
	* IsFraming 是否分帧，某些函数基于性能考虑，可能需要下帧执行，减少本帧占用时间
	*/
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "AsyncFunction"))
	USimpleCodePipeline* PushAsyncFunction(const FPipelineAsyncFunction& AsyncFunction, bool IsFraming = false);

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
		union {
			FPipelineSyncFunction SyncFunction;
			FPipelineAsyncFunction AsyncFunction;
		};

		FPipelineFunction();
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
};