// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/EventHandlerInterface.h"
#include "Base/ECS/SystemBase.h"
#include "ScenarioSystem.generated.h"

class UScenarioComponent;

/*
* @brief 剧情管理
*/
UCLASS()
class GAMEFRAMEWORK_API UScenarioSystem : public USystemBase, public IEventHandlerInterface {
    GENERATED_BODY()

public:
    virtual void Initialize(UCoreGameInstance* InGameInstance) override;
    virtual void Uninitialize() override;

    /**
    * 播放剧情
    */
    class UAsyncPlayScenario* PlayScenario(UScenarioComponent* ScenarioComponent, class UScenario* Scenario);

    /**
    * 剧情步进
    */
    void Step(UScenarioComponent* ScenarioComponent, int ChildIndex);

    /**
    * 执行当前剧情的所有动作
    */
    UFUNCTION(BlueprintCallable)
    void ExecuteCurrentSceneActions(UScenarioComponent* ScenarioComponent);

private:
    UPROPERTY()
    class UPlayScenarioPredicate* PlayScenarioPredicate;

    /** 覆写事件监听 */
    virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes() override;
    virtual void OnEvent(UCoreGameInstance* IngGameInstance, UGameEventBase* HandleEvent) override;

    void PlayScenarioAfterLoaded(FString EntityID, FString ScenarioPath);
};