// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Quest/QuestTypes.h"
#include "Quest.generated.h"

/**
* @brief 任务
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UQuest : public UObject
{
	GENERATED_UCLASS_BODY()

	/**
	* id
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Quest", meta = (DisplayName = "任务ID"))
	FGuid ID;

	/**
	* 名字
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Quest", meta = (DisplayName = "任务名字"))
	FString QuestName;

	/**
	* 任务类型
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Quest", meta = (DisplayName = "任务类型"))
	EQuestTypeEnum QuestType;

	/**
	* 前置条件
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced, Category = "Quest", meta = (DisplayName = "前置条件"))
	TArray<TObjectPtr<class UCoreCondition>> PreConditionList;

	/**
	* 任务详情
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Quest", meta = (DisplayName = "任务详情"))
	TSoftObjectPtr<class UQuestDetail> QuestDetail;

	/**
	* 前置任务
	*/
	UPROPERTY(Transient)
	TArray<class UQuest*> PreQuests;

	/**
	* 后续任务
	*/
	UPROPERTY()
	TArray<class UQuest*> FollowQuests;
};