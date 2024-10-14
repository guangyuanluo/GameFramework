// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/SystemBase.h"
#include "ScenarioSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScenarioPlayStartDelegate, class UAsyncPlayScenario*, ScenarioToPlay, class UScenarioNode*, NodeToPlay, UObject*, ScenarioContextToPlay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScenarioPlayEndDelegate, class UAsyncPlayScenario*, ScenarioToPlay, class UScenarioNode*, NodeToPlay, class UScenarioNode*, NextNodeToPlay);

USTRUCT()
struct FPlayScenarioQueueItem {
    GENERATED_BODY()

    UPROPERTY()
    class UAsyncPlayScenario* PlayScenario;

    UPROPERTY()
    UObject* Context;
};
/*
* @brief 剧情管理
*/
UCLASS()
class GAMEFRAMEWORK_API UScenarioSystem : public USystemBase {
    GENERATED_BODY()

public:
    virtual void Initialize(UCoreGameInstance* InGameInstance) override;
    virtual void Uninitialize() override;

    /**
    * 开始播放剧情
    */
    UPROPERTY(BlueprintAssignable)
    FOnScenarioPlayStartDelegate OnScenarioPlayStartDelegate;

    /**
    * 剧情节点播放结束
    */
    UPROPERTY(BlueprintAssignable)
    FOnScenarioPlayEndDelegate OnScenarioPlayEndDelegate;

    /**
    * 播放剧情
    */
    UFUNCTION(BlueprintCallable)
    class UAsyncPlayScenario* PlayScenario(class UScenario* Scenario, UObject* Context = nullptr);

    /**
    * 剧情步进
    */
    UFUNCTION(BlueprintCallable)
    void Step(int ChildIndex);

    /**
    * 执行当前剧情的所有动作
    */
    UFUNCTION(BlueprintCallable)
    void ExecuteCurrentSceneActions();

private:
    /**
    * 剧情优先级队列
    */
    UPROPERTY()
    TArray<FPlayScenarioQueueItem> PlayScenarioQueue;

    /**
    * 当前播放的剧情
    */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    class UAsyncPlayScenario* CurrentPlayScenario;

    /**
    * 当前播放的剧情节点
    */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    class UScenarioNode* CurrentPlayScenarioNode;

    /**
    * 当前剧情播放的上下文
    */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    UObject* CurrentScenarioContext;

    UPROPERTY()
    class UPlayScenarioPredicate* PlayScenarioPredicate;
};