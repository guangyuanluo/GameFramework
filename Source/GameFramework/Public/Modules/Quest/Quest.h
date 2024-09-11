// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionList.h"
#include "Quest.generated.h"

//任务类型
UENUM(BlueprintType, meta = (DisplayName = "任务类型"))
enum class QuestTypeEnum :uint8 //设置uint8类型  
{
	QuestType_1,
	QuestType_2,
	QuestType_3,
	QuestType_4,
	QuestType_5,
	QuestType_6,
	QuestType_7,
	QuestType_8,
	QuestType_9,
};

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quest", meta = (DisplayName = "任务名字"))
	FString QuestName;

	/**
	* 任务类型
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quest", meta = (DisplayName = "任务类型"))
	QuestTypeEnum QuestType;

	/**
	* 前置条件
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quest", meta = (DisplayName = "前置条件"))
	FCoreConditionList PreConditionList;

	/**
	* 任务详情
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quest", meta = (DisplayName = "任务详情"))
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

	//~ Begin UObject Interface
#if WITH_EDITOR
	virtual void PostEditImport() override;
	virtual void PostDuplicate(bool bDuplicateForPIE) override;
#endif
	// End UObject
};