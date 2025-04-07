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
}
void UScenarioSystem::Uninitialize() {
    Super::Uninitialize();

    PlayScenarioPredicate = nullptr;
}

class UAsyncPlayScenario* UScenarioSystem::PlayScenario(class UScenario* Scenario) {
    UAsyncPlayScenario* PlayScenario = NewObject<UAsyncPlayScenario>();
    PlayScenario->Scenario = Scenario;

    FPlayScenarioQueueItem QueueItem;
    QueueItem.PlayScenario = PlayScenario;

    PlayScenarioQueue.HeapPush(QueueItem, FPlayScenarioPredicate(PlayScenarioPredicate));
    if (CurrentPlayScenario == nullptr) {
        //当前没有待播放的剧情，直接开始
        Step(0);
    }

    return PlayScenario;
}

void UScenarioSystem::Step(int ChildIndex) {
    if (CurrentPlayScenario == nullptr) {
        if (PlayScenarioQueue.Num() != 0) {
            FPlayScenarioQueueItem Top;
            PlayScenarioQueue.HeapPop(Top, FPlayScenarioPredicate(PlayScenarioPredicate));
            CurrentPlayScenario = Top.PlayScenario;
        }
    }
    if (CurrentPlayScenario) {
        if (CurrentPlayScenarioNode == nullptr) {
            CurrentPlayScenarioNode = CurrentPlayScenario->Scenario->RootScenario;
            OnScenarioPlayStartDelegate.Broadcast(CurrentPlayScenario, CurrentPlayScenarioNode);
        }
        else {
            auto FollowNodeNum = CurrentPlayScenarioNode->FollowScenarioNodes.Num();
            if (FollowNodeNum == 0) {
                //这里表示剧情播放完成
                CurrentPlayScenario->OnComplete.Broadcast(CurrentPlayScenario->Scenario, 0);
                OnScenarioPlayEndDelegate.Broadcast(CurrentPlayScenario, CurrentPlayScenarioNode, nullptr);

                CurrentPlayScenario = nullptr;
                CurrentPlayScenarioNode = nullptr;
            }
            else if (FollowNodeNum == 1) {
                auto NextNode = CurrentPlayScenarioNode->FollowScenarioNodes[0];
                OnScenarioPlayEndDelegate.Broadcast(CurrentPlayScenario, CurrentPlayScenarioNode, NextNode);
                CurrentPlayScenarioNode = NextNode;
                OnScenarioPlayStartDelegate.Broadcast(CurrentPlayScenario, CurrentPlayScenarioNode);
            }
            else {
                if (ChildIndex >= FollowNodeNum) {
                    UE_LOG(GameCore, Error, TEXT("剧情节点异常，传入的索引超过了子节点的数量，蓝图路径:%s"), *CurrentPlayScenario->Scenario->GetClass()->GetFullName());
                }
                else {
                    auto NextNode = CurrentPlayScenarioNode->FollowScenarioNodes[ChildIndex];
                    OnScenarioPlayEndDelegate.Broadcast(CurrentPlayScenario, CurrentPlayScenarioNode, NextNode);
                    CurrentPlayScenarioNode = NextNode;
                    OnScenarioPlayStartDelegate.Broadcast(CurrentPlayScenario, CurrentPlayScenarioNode);
                }
            }
        }
        if (CurrentPlayScenarioNode) {
            auto ScenarioNodeReturn = Cast<UScenarioNodeReturn>(CurrentPlayScenarioNode);
            if (ScenarioNodeReturn) {
                //表示现在是返回节点，终止
                //剧情完成
                CurrentPlayScenario->OnComplete.Broadcast(CurrentPlayScenario->Scenario, ScenarioNodeReturn->ReturnBranch);
                OnScenarioPlayEndDelegate.Broadcast(CurrentPlayScenario, CurrentPlayScenarioNode, nullptr);

                CurrentPlayScenario = nullptr;
                CurrentPlayScenarioNode = nullptr;

                //当前剧情播放完，继续下一个
                if (PlayScenarioQueue.Num() > 0) {
                    Step(0);
                }
            }
        }
    }
}

void UScenarioSystem::ExecuteCurrentSceneActions() {
    if (CurrentPlayScenario && CurrentPlayScenarioNode && CurrentPlayScenarioNode->Actions.Num() > 0) {
        for (auto Action : CurrentPlayScenarioNode->Actions) {
            Action->Execute(CurrentPlayScenario->Scenario, CurrentPlayScenarioNode);
        }
    }
}