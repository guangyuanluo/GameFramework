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
    TSubclassOf<UPlayScenarioPredicate> PredicateClass = StaticLoadClass(UPlayScenarioPredicate::StaticClass(), NULL, *ScenarioSetting->PlayScenarioPredicateClass.ToString());
    if (!PredicateClass) {
        PredicateClass = UPlayScenarioPredicate::StaticClass();
    }
    PlayScenarioPredicate = NewObject<UPlayScenarioPredicate>(this, PredicateClass);
}
void UScenarioSystem::Uninitialize() {
    Super::Uninitialize();

    PlayScenarioPredicate = nullptr;
}

class UAsyncPlayScenario* UScenarioSystem::PlayScenario(class UScenario* Scenario, UObject* Context) {
    UAsyncPlayScenario* PlayScenario = NewObject<UAsyncPlayScenario>();
    PlayScenario->Scenario = Scenario;

    FPlayScenarioQueueItem QueueItem;
    QueueItem.PlayScenario = PlayScenario;
    QueueItem.Context = Context;

    PlayScenarioQueue.HeapPush(QueueItem, FPlayScenarioPredicate(PlayScenarioPredicate));

    return PlayScenario;
}

void UScenarioSystem::Step(int ChildIndex) {
    if (CurrentPlayScenario == nullptr) {
        if (PlayScenarioQueue.Num() != 0) {
            FPlayScenarioQueueItem Top;
            PlayScenarioQueue.HeapPop(Top, FPlayScenarioPredicate(PlayScenarioPredicate));
            CurrentPlayScenario = Top.PlayScenario;
            CurrentScenarioContext = Top.Context;
        }
    }
    if (CurrentPlayScenario) {
        if (CurrentPlayScenarioNode == nullptr) {
            CurrentPlayScenarioNode = CurrentPlayScenario->Scenario->RootScenario;
        }
        else {
            auto FollowNodeNum = CurrentPlayScenarioNode->FollowScenarioNodes.Num();
            if (FollowNodeNum == 0) {
                //这里表示剧情播放完成
                CurrentPlayScenario->OnComplete.Broadcast(CurrentPlayScenario->Scenario, 0);

                CurrentPlayScenario = nullptr;
                CurrentPlayScenarioNode = nullptr;
                CurrentScenarioContext = nullptr;
            }
            else if (FollowNodeNum == 1) {
                CurrentPlayScenarioNode = CurrentPlayScenarioNode->FollowScenarioNodes[0];
            }
            else {
                if (ChildIndex >= FollowNodeNum) {
                    UE_LOG(GameCore, Error, TEXT("剧情节点异常，传入的索引超过了子节点的数量，蓝图路径:%s"), *CurrentPlayScenario->Scenario->GetClass()->GetFullName());
                }
                else {
                    CurrentPlayScenarioNode = CurrentPlayScenarioNode->FollowScenarioNodes[ChildIndex];
                }
            }
        }
        if (CurrentPlayScenarioNode) {
            auto ScenarioNodeReturn = Cast<UScenarioNodeReturn>(CurrentPlayScenarioNode);
            if (ScenarioNodeReturn) {
                //表示现在是返回节点，终止
                //剧情完成
                CurrentPlayScenario->OnComplete.Broadcast(CurrentPlayScenario->Scenario, ScenarioNodeReturn->ReturnBranch);

                CurrentPlayScenario = nullptr;
                CurrentPlayScenarioNode = nullptr;
                CurrentScenarioContext = nullptr;
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