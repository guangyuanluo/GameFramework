// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/ConditionTriggerHandler.h"
#include "AcceptableQuest.generated.h"

class UCoreConditionProgress;
class UQuestComponent;
class UQuest;

/**
 * 可接受的任务
 */
UCLASS()
class GAMEFRAMEWORK_API UAcceptableQuest : public UObject
{
public:
	GENERATED_BODY()

	void Initialize(UQuest* InQuestPtr, UQuestComponent* InQuestComponent);
	void Uninitialize();
	void StartListen();
	void StopListen();

	UPROPERTY()
	UQuestComponent* QuestComponent;

	/**
	* @brief 获得任务模板id
	*/
	UFUNCTION(BlueprintPure, Category = "QuestSystem")
	const FGuid& GetID() const;

	/**
	* @brief 获取任务进度
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	const TArray<UCoreConditionProgress*>& GetQuestProgresses() const;

	/**
	* @brief 是否完成
	*/
	bool IsComplete() const;	

private:
	/** 基本信息 */
	TWeakObjectPtr<UQuest> QuestPtr;					///< 任务引用行
	/** 完成条件进度 */
	UPROPERTY()
	TArray<UCoreConditionProgress*> QuestProgresses;
	/** 条件触发句柄 */
	FConditionTriggerHandler ConditionTriggerHandler;

	/**
	* 所有进度满足
	*/
	UFUNCTION()
	void OnAllProgressesSatisfy();
};