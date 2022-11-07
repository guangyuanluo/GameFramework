// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/Subsystem/GameInstanceSubsystemBase.h"
#include "QuestForestSubsystem.generated.h"

class UQuestTree;
class UQuest;

/*
* @brief 任务森林子系统
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UQuestForestSubsystem : public UGameInstanceSubsystemBase {
public:
	GENERATED_BODY()

	virtual void Init_Implementation() override;

	/**
	* 任务树加入森林
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AddToForest(UQuestTree* InQuestTree);

	/**
	* 任务树移出森林
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void RemoveFromForest(UQuestTree* InQuestTree);

	/**
	* 获取所有任务树
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	const TMap<FGuid, UQuestTree*>& GetAllQuestTreeMap() const;

	/**
	* 获取指定id的任务树
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	UQuestTree* GetQuestTreeByID(const FGuid& ID) const;

	/**
	* 获取指定id的任务
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	UQuest* GetQuestByID(const FGuid& ID) const;

private:
	/**
	* 任务树map
	*/
	UPROPERTY()
	TMap<FGuid, UQuestTree*> QuestTreeMap;

	/**
	* 任务map
	*/
	UPROPERTY()
	TMap<FGuid, UQuest*> QuestMap;
};