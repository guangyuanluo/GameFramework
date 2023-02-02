// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AdvanceObjectPromise.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnObjectPromiseSuccess, UObject*, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnObjectPromiseFail, const FString&, FailureReason);

/**
* result是object的promise
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UAdvanceObjectPromise : public UObject {
	GENERATED_UCLASS_BODY()

public:
	~UAdvanceObjectPromise();

	/**
	* 尝试设置结果
	*/
	UFUNCTION(BlueprintCallable)
	void TrySuccess(UObject* InResult);

	/**
	* 尝试设置失败
	*/
	UFUNCTION(BlueprintCallable)
	void TryFailure(const FString& InFailureReason);

	/**
	* 是否已经有结果了
	*/
	UFUNCTION(BlueprintCallable)
	bool IsReady() const;

	/**
	* 是否成功
	*/
	UFUNCTION(BlueprintCallable)
	bool IsSuccess() const;

	/**
	* 获取结果
	*/
	UFUNCTION(BlueprintCallable)
	UObject* GetResult() const;

	/**
	* 获取失败原因
	*/
	UFUNCTION(BlueprintCallable)
	const FString& GetFailureReason() const;

	/**
	* 添加成功回调
	*/
	UFUNCTION(BlueprintCallable)
	void AddSuccessListener(FOnObjectPromiseSuccess SuccessCallback);

	/**
	* 添加失败回调
	*/
	UFUNCTION(BlueprintCallable)
	void AddFailureListener(FOnObjectPromiseFail FailureCallback);

	/**
	 * 绑定到另外一个promise上，另一个promise成功自己就成功，另一个promise失败自己就失败
	 */
	UFUNCTION(BlueprintCallable)
	void BindOtherPromise(UAdvanceObjectPromise* OtherPromise);

	/**
	* 设置超时时间
	*/
	UFUNCTION(BlueprintCallable)
	void SetTimeout(float Timeout);

private:
	UPROPERTY()
	UObject* Result;

	bool HaveSet = false;
	FString FailureReason;
	FTimerHandle TimeoutHandle;

	TArray<FOnObjectPromiseSuccess> OnSuccess;
	TArray<FOnObjectPromiseFail> OnFailure;

	TSharedPtr<TPromise<bool>> Promise;
	TFuture<bool> Future;

	UFUNCTION()
	void BindOtherPromiseSuccessCallback(UObject* InResult);

	UFUNCTION()
	void BindOtherPromiseFailureCallback(const FString& InFailureReason);

	void TimeoutCallback();
	void ClearTimeout();
};