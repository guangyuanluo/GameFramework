// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/ConditionTriggerHandler.h"
#include "ExecutingQuest.generated.h"

class UCoreConditionProgress;
class UQuestComponent;
class UQuest;
class UQuestDetailNode;
class UScenario;


/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UExecutingQuest : public UObject
{
public:
	GENERATED_BODY()

	void Initialize(UQuest* InQuest);
	void Uninitiliaize();

	/**
	* @brief 获得任务id
	*/
	UFUNCTION(BlueprintPure, Category = "QuestSystem")
	const FGuid& GetID() const;

	/**
	* @brief 获得当前任务节点
	*/
	UFUNCTION(BlueprintPure, Category = "QuestSystem")
	UQuestDetailNode* GetCurrentNode();

	UFUNCTION(BlueprintPure, Category = "QuestSystem")
	UQuest* GetQuest();

	/**
	* @brief 获取任务进度
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	const TArray<UCoreConditionProgress*>& GetQuestProgresses() const;

	/**
	* @brief 获取任务奖励
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	const TArray<UCoreReward*>& GetQuestRewards() const;

	/**
	* @brief 是否完成
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	bool IsComplete() const;

	/**
	* 获取属于的QuestComponent
	*/
	UFUNCTION(BlueprintPure, Category = "QuestSystem")
	UQuestComponent* GetQuestComponent();

	/**
	* 是不是最后一个节点
	*/
	UFUNCTION(BlueprintPure, Category = "QuestSystem")
	bool IsLastNode();

	/**
	* 步进下个节点
	*/
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void StepNextNode(int StepIndex);

	/**
	* 设置剧情ID
	*/
	void SetNodeID(const FGuid& InNodeID);

	//继承
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override;

private:
	TSharedPtr<struct FStreamableHandle> ScenarioLoadedHandle;

	/**
	* 任务id
	*/
    UPROPERTY(ReplicatedUsing = OnRep_QuestID)
	FGuid ID;

	/**
	* 节点id
	*/
	UPROPERTY(ReplicatedUsing = OnRep_NodeID)
	FGuid NodeID;

	/**
	* 任务
	*/
	TWeakObjectPtr<UQuest> Quest;

	/** 完成条件进度 */
	UPROPERTY(ReplicatedUsing = OnRep_Progress)
	TArray<UCoreConditionProgress*> QuestProgresses;

	/**
	* 完成奖励
	*/
	UPROPERTY(ReplicatedUsing = OnRep_Rewards)
	TArray<UCoreReward*> QuestRewards;

	FConditionTriggerHandler ConditionTriggerHandler;
	/**
	* 所有进度完成
	*/
	UFUNCTION()
	void OnAllProgressesSatisfy(FConditionTriggerHandler Handler);

	UFUNCTION()
	void OnRep_QuestID();

	UFUNCTION()
	void OnRep_NodeID();

	UFUNCTION()
	void OnRep_Progress(const TArray<UCoreConditionProgress*>& OldProgresses);

	UFUNCTION()
	void OnRep_Rewards();

	void NotifyPlayScenario();
	void NotifyPlayScenarioAfterLoaded(FSoftObjectPath LoadScenarioPath);
	UFUNCTION()
	void PlayScenarioCompleted(UScenario* PlayScenario, int ReturnIndex);

	void SetNode(UQuestDetailNode* InNode);

	UFUNCTION()
	void OnProgressPostNetReceive(UCoreConditionProgress* Progress);
};