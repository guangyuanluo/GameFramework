// Fill out your copyright notice in the Description page of Project Settings.

#include "ScenarioSystem.h"
#include "AsyncPlayScenario.h"
#include "PlayScenarioPredicate.h"
#include "ScenarioSetting.h"
#include "Scenario.h"
#include "ScenarioNode.h"
#include "UE4LogImpl.h"
#include "ScenarioNodeReturn.h"
#include "ScenarioAction.h"
#include "ScenarioComponent.h"
#include "GameEntityManager.h"
#include "CoreGameInstance.h"
#include "CoreCharacterStateBase.h"
#include "Engine/AssetManager.h"
#include "GameSystemManager.h"
#include "EventSystem.h"

struct FPlayScenarioPredicate {
    FPlayScenarioPredicate(UPlayScenarioPredicate* InPlayScenarioPredicate) {
        PlayScenarioPredicate = InPlayScenarioPredicate;
    }
    bool operator()(const FPlayScenarioQueueItem& A, const FPlayScenarioQueueItem& B) const {
        // Inverted compared to std::priority_queue - higher priorities float to the top
        return PlayScenarioPredicate->Compare(A.PlayScenario, B.PlayScenario);
    }

    UPlayScenarioPredicate* PlayScenarioPredicate;
};

void UScenarioSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);
    
    const UScenarioSetting* ScenarioSetting = GetDefault<UScenarioSetting>();
    TSubclassOf<UPlayScenarioPredicate> PredicateClass = UPlayScenarioPredicate::StaticClass();
    FString PredicateClassPath = ScenarioSetting->PlayScenarioPredicateClass.ToString();
    if (!PredicateClassPath.IsEmpty()) {
        TSubclassOf<UPlayScenarioPredicate> LoadClass = StaticLoadClass(UPlayScenarioPredicate::StaticClass(), NULL, *PredicateClassPath);
        if (LoadClass) {
            PredicateClass = LoadClass;
        }
    }
    PlayScenarioPredicate = NewObject<UPlayScenarioPredicate>(this, PredicateClass);

    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->RegistEventHandler(this);
}
void UScenarioSystem::Uninitialize() {
    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->UnRegistEventHandler(this);

    Super::Uninitialize();

    PlayScenarioPredicate = nullptr;
}

class UAsyncPlayScenario* UScenarioSystem::PlayScenario(UScenarioComponent* ScenarioComponent, class UScenario* Scenario) {
    UAsyncPlayScenario* PlayScenario = NewObject<UAsyncPlayScenario>();
    PlayScenario->Scenario = Scenario;

    FPlayScenarioQueueItem QueueItem;
    QueueItem.PlayScenario = PlayScenario;

    ScenarioComponent->PlayScenarioQueue.HeapPush(QueueItem, FPlayScenarioPredicate(PlayScenarioPredicate));
    if (ScenarioComponent->CurrentAsyncPlayScenario == nullptr) {
        //当前没有待播放的剧情，直接开始
        Step(ScenarioComponent, 0);
    }

    return PlayScenario;
}

void UScenarioSystem::Step(UScenarioComponent* ScenarioComponent, int ChildIndex) {
    if (ScenarioComponent->CurrentAsyncPlayScenario == nullptr) {
        if (ScenarioComponent->PlayScenarioQueue.Num() != 0) {
            FPlayScenarioQueueItem Top;
            ScenarioComponent->PlayScenarioQueue.HeapPop(Top, FPlayScenarioPredicate(PlayScenarioPredicate));
            ScenarioComponent->CurrentAsyncPlayScenario = Top.PlayScenario;
        }
    }
    if (ScenarioComponent->CurrentAsyncPlayScenario) {
        if (ScenarioComponent->CurrentPlayScenarioNode == nullptr) {
            ScenarioComponent->SetCurrent(ScenarioComponent->CurrentAsyncPlayScenario->Scenario->RootScenario);
        }
        else {
            auto FollowNodeNum = ScenarioComponent->CurrentPlayScenarioNode->FollowScenarioNodes.Num();
            if (FollowNodeNum == 0) {
                //这里表示剧情播放完成
                ScenarioComponent->CurrentAsyncPlayScenario->OnComplete.Broadcast(ScenarioComponent->CurrentAsyncPlayScenario->Scenario, 0);

                ScenarioComponent->CurrentAsyncPlayScenario = nullptr;
                ScenarioComponent->SetCurrent(nullptr);
            }
            else if (FollowNodeNum == 1) {
                auto NextNode = ScenarioComponent->CurrentPlayScenarioNode->FollowScenarioNodes[0];

                ScenarioComponent->SetCurrent(NextNode);
            }
            else {
                if (ChildIndex >= FollowNodeNum) {
                    UE_LOG(GameCore, Error, TEXT("剧情节点异常，传入的索引超过了子节点的数量，蓝图路径:%s"), *ScenarioComponent->CurrentAsyncPlayScenario->Scenario->GetClass()->GetFullName());
                }
                else {
                    auto NextNode = ScenarioComponent->CurrentPlayScenarioNode->FollowScenarioNodes[ChildIndex];
                    ScenarioComponent->SetCurrent(NextNode);
                }
            }
        }
        if (ScenarioComponent->CurrentAsyncPlayScenario) {
            auto ScenarioNodeReturn = Cast<UScenarioNodeReturn>(ScenarioComponent->CurrentPlayScenarioNode);
            if (ScenarioNodeReturn) {
                //表示现在是返回节点，终止
                //剧情完成
                ScenarioComponent->CurrentAsyncPlayScenario->OnComplete.Broadcast(ScenarioNodeReturn->ScenarioAsset, ScenarioNodeReturn->ReturnBranch);

                ScenarioComponent->CurrentAsyncPlayScenario = nullptr;
                ScenarioComponent->SetCurrent(nullptr);

                //当前剧情播放完，继续下一个
                if (ScenarioComponent->PlayScenarioQueue.Num() > 0) {
                    Step(ScenarioComponent, 0);
                }
            }
        }
    }
}

void UScenarioSystem::ExecuteCurrentSceneActions(UScenarioComponent* ScenarioComponent) {
    if (ScenarioComponent->CurrentPlayScenarioNode && ScenarioComponent->CurrentPlayScenarioNode->Actions.Num() > 0) {
        for (auto Action : ScenarioComponent->CurrentPlayScenarioNode->Actions) {
            Action->Execute(ScenarioComponent->CurrentPlayScenarioNode);
        }
    }
}

TArray<TSubclassOf<class UGameEventBase>> UScenarioSystem::GetHandleEventTypes() {
    return {
        UPlayScenarioRequesEvent::StaticClass(),
        UStepScenarioRequesEvent::StaticClass(),
    };
}

void UScenarioSystem::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
    if (HandleEvent->IsA(UPlayScenarioRequesEvent::StaticClass())) {
        auto Request = Cast<UPlayScenarioRequesEvent>(HandleEvent);
        UAssetManager::GetStreamableManager().RequestAsyncLoad(Request->ScenarioPath, FStreamableDelegate::CreateUObject(this, &UScenarioSystem::PlayScenarioAfterLoaded, Request->EntityId, Request->ScenarioPath));
    }
    else if (HandleEvent->IsA(UStepScenarioRequesEvent::StaticClass())) {
        auto Request = Cast<UStepScenarioRequesEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto Character = Cast<APawn>(Entity.GetObject());
            if (Character) {
                auto PlayerState = Cast<ACoreCharacterStateBase>(Character->GetPlayerState());
                if (PlayerState && PlayerState->ScenarioComponent) {
                    Step(PlayerState->ScenarioComponent, Request->ChildIndex);
                }
            }
        }
    }
}

void UScenarioSystem::PlayScenarioAfterLoaded(FString EntityID, FString ScenarioPath) {
    auto Entity = GameInstance->GameEntityManager->GetEntityById(EntityID);
    if (Entity) {
        auto Character = Cast<APawn>(Entity.GetObject());
        if (Character) {
            auto PlayerState = Cast<ACoreCharacterStateBase>(Character->GetPlayerState());
            if (PlayerState && PlayerState->ScenarioComponent) {
                //异步加载回调完成可以同步加载
                auto Scenario = Cast<UScenario>(StaticLoadObject(UScenario::StaticClass(), nullptr, *ScenarioPath));
                if (Scenario) {
                    PlayScenario(PlayerState->ScenarioComponent, Scenario);
                }
            }
        }
    }
}