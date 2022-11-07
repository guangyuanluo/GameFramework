// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/EventHandlerInterface.h"
#include "Base/ECS/SystemBase.h"
#include "QuestSystem.generated.h"

class UQuestComponent;
class UExecutingQuest;
class UCoreGameInstance;
struct FQuestConfigTableRow;

/*
* @brief 任务系统
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UQuestSystem : public USystemBase, public IEventHandlerInterface {
public:
	GENERATED_BODY()

	virtual void Initialize(UCoreGameInstance* InGameInstance) override;
    virtual void Uninitialize() override;

	/**
	* @brief 接受任务
	*/
	void AcceptQuest(UQuestComponent* QuestComponent, const FGuid& ID);

	/**
	* @brief 推动任务
	*/
	bool PushQuest(UQuestComponent* QuestComponent, const FGuid& ID, int32 StepIndex, FString& Error);

private:
	void AddExecutinQuestAfterLoaded(UQuestComponent* QuestComponent, const FGuid ID);

    /** 覆写事件监听 */
    virtual TArray<UClass*> GetHandleEventTypes_Implementation() override;
    virtual void OnEvent_Implementation(UCoreGameInstance* IngGameInstance, UGameEventBase* HandleEvent) override;
};