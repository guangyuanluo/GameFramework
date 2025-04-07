// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Modules/Events/GameEventBase.h"
#include "Base/ECS/GameEntityComponent.h"
#include "QuestComponent.generated.h"


UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEFRAMEWORK_API UQuestComponent : public UGameEntityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
    UPROPERTY(Category = "Quest", EditAnywhere, BlueprintReadOnly)
    TMap<FGuid, class UAcceptableQuest*> AcceptQuests;		    ///< 可接受的任务

	UPROPERTY(ReplicatedUsing = OnQuestChanged, Category = "Quest", EditAnywhere, BlueprintReadOnly)
	TArray<class UExecutingQuest*> ExecutingQuests;				///< 执行中的任务

	UPROPERTY(ReplicatedUsing = OnFinishQuestChanged)
	TArray<FGuid> FinishQuests;								    ///< 已完成任务

	/*接受任务*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AcceptQuest(const FGuid& ID);

	/*推动任务*/
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void PushQuest(const FGuid& ID, int StepIndex);

    /* 根据剧情找到关联的执行中任务 */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    class UExecutingQuest* FindExecutingQuestFromScenario(class UScenario* ScenarioAsset) const;

    UFUNCTION()
    void OnQuestChanged();

    void NotifyQuestChangedAfterLoaded();

    UFUNCTION()
    void OnFinishQuestChanged();

    UFUNCTION()
    void RefreshAcceptableQuests();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
    bool bDirtyQuests = false;
    TSharedPtr<struct FStreamableHandle> QuestLoadedHandle;

    void RefreshAcceptQuests();
    void NotifyServerNextTickHandle();
    void StopServerTick();
};


/*************事件定义**************/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UOnQuestRefreshEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    class UQuestComponent* QuestComponent;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UOnFinishQuestsRefreshEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
        class UQuestComponent* QuestComponent;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UOnAcceptQuestIdsRefreshEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    class UQuestComponent* QuestComponent;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UAcceptQuestRequesEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
        FString EntityId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
        FGuid ID;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPushQuestRequesEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
        FString EntityId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
        FGuid ID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
        int32 StepIndex;
};