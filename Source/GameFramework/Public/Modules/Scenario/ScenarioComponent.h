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
    * ����ڵ����
    */
    UPROPERTY(BlueprintAssignable)
    FOnScenarioUpdateDelegate OnScenarioUpdateDelegate;

    /**
    * ���ž���
    */
    UFUNCTION(BlueprintCallable)
    void PlayScenario(TSoftObjectPtr<class UScenario> Scenario);

    /**
    * ���鲽��
    */
    UFUNCTION(BlueprintCallable)
    void Step(int ChildIndex);

private:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    friend class UScenarioSystem;
    /**
    * �������ȼ�����
    */
    UPROPERTY()
    TArray<FPlayScenarioQueueItem> PlayScenarioQueue;

    /**
    * ��ǰ���ŵľ����첽�ص�
    */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    class UAsyncPlayScenario* CurrentAsyncPlayScenario;

    /**
    * ��ǰ���ŵľ���ڵ�
    */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing = OnRep_CurrentPlayScenarioNode, meta = (AllowPrivateAccess = "true"))
    class UScenarioNode* CurrentPlayScenarioNode;

    void SetCurrent(class UScenarioNode* InScenarioNode);

    UFUNCTION()
    void OnRep_CurrentPlayScenarioNode();
};


/*************�¼�����**************/
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