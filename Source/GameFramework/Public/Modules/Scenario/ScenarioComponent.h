// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/GameEventBase.h"
#include "Base/ECS/GameEntityComponent.h"
#include "ScenarioComponent.generated.h"

USTRUCT()
struct FPlayScenarioQueueItem {
    GENERATED_BODY()

    UPROPERTY()
    class UAsyncPlayScenario* PlayScenario;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScenarioUpdateDelegate, class UScenarioNode*, NodeToPlay);

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEFRAMEWORK_API UScenarioComponent : public UGameEntityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScenarioComponent();

    /**
    * 剧情节点更新
    */
    UPROPERTY(BlueprintAssignable)
    FOnScenarioUpdateDelegate OnScenarioUpdateDelegate;

    /**
    * 播放剧情
    */
    UFUNCTION(BlueprintCallable)
    void PlayScenario(TSoftObjectPtr<class UScenario> Scenario);

    /**
    * 剧情步进
    */
    UFUNCTION(BlueprintCallable)
    void Step(int ChildIndex);

private:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    friend class UScenarioSystem;
    /**
    * 剧情优先级队列
    */
    UPROPERTY()
    TArray<FPlayScenarioQueueItem> PlayScenarioQueue;

    /**
    * 当前播放的剧情异步回调
    */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    class UAsyncPlayScenario* CurrentAsyncPlayScenario;

    /**
    * 当前播放的剧情节点
    */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = OnRep_CurrentPlayScenarioNode, meta = (AllowPrivateAccess = "true"))
    class UScenarioNode* CurrentPlayScenarioNode;

    void SetCurrent(class UScenarioNode* InScenarioNode);

    UFUNCTION()
    void OnRep_CurrentPlayScenarioNode();
};


/*************事件定义**************/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayScenarioRequesEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString EntityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString ScenarioPath;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UStepScenarioRequesEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString EntityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    int ChildIndex;
};