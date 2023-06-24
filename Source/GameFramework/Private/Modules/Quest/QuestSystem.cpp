// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestSystem.h"
#include "CoreGameInstance.h"
#include "UE4LogImpl.h"
#include "EventSystem.h"
#include "CompleteQuestEvent.h"
#include "PlayerQuestFinishCondition.h"
#include "ConditionSystem.h"
#include "AcceptableQuest.h"
#include "GameFrameworkUtils.h"
#include "PlayerComponent.h"
#include "QuestComponent.h"
#include "ExecutingQuest.h"
#include "CoreConditionProgress.h"
#include "CoreReward.h"
#include "GameEntityManager.h"
#include "GameSystemManager.h"
#include "CoreCharacterStateBase.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "QuestDetail.h"
#include "QuestForestSubsystem.h"
#include "Quest.h"
#include "CoreReward.h"

void UQuestSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);
	GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->RegistEventHandler(this);
}

void UQuestSystem::Uninitialize() {
    
}

void UQuestSystem::AcceptQuest(UQuestComponent* QuestComponent, const FGuid& ID) {
    if (QuestComponent->FinishQuests.Contains(ID)) {
        UE_LOG(GameCore, Warning, TEXT("接受任务%s已完成"), *ID.ToString());
        return;
    }
    for (int Index = 0; Index < QuestComponent->ExecutingQuests.Num(); ++Index) {
        auto Quest = QuestComponent->ExecutingQuests[Index];
        if (Quest->GetID() == ID) {
            UE_LOG(GameCore, Warning, TEXT("接受任务%s已接受"), *ID.ToString());

            return;
        }
    }
    UQuestForestSubsystem* QuestForestSubsystem = GameInstance->GetSubsystem<UQuestForestSubsystem>();
    auto Quest = QuestForestSubsystem->GetQuestByID(ID);
    if (!Quest) {
        UE_LOG(GameCore, Warning, TEXT("接受任务非法ID:%s"), *ID.ToString());

        return;
    }

    UAcceptableQuest* AcceptableQuest = NewObject<UAcceptableQuest>();
    AcceptableQuest->Initialize(Quest, QuestComponent);
    if (!AcceptableQuest->IsComplete()) {
        UE_LOG(GameCore, Warning, TEXT("接受任务失败:%s"), *ID.ToString());

        return;
    }
    //预加载资源
    UAssetManager::GetStreamableManager().RequestAsyncLoad(Quest->QuestDetail.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UQuestSystem::AddExecutinQuestAfterLoaded, QuestComponent, ID));
}

bool UQuestSystem::PushQuest(UQuestComponent* QuestComponent, const FGuid& ID, int32 StepIndex, FString& Error) {
    int32 FindIndex = -1;
    UExecutingQuest* FindQuest = nullptr;
    for (int Index = 0; Index < QuestComponent->ExecutingQuests.Num(); ++Index) {
        if (QuestComponent->ExecutingQuests[Index]->GetQuest()->ID == ID) {
            FindQuest = QuestComponent->ExecutingQuests[Index];
            FindIndex = Index;
            break;
        }
    }

    if (!FindQuest) {
        Error = TEXT("提交任务不在进行中");
        return false;
    }
    if (!FindQuest->IsComplete()) {
        Error = TEXT("提交任务条件不满足");
        return false;
    }
    if (FindQuest->IsLastNode()) {
        //todo，任务进度完成
        const auto& QuestProgresses = FindQuest->GetQuestProgresses();
        for (auto QuestProgress : QuestProgresses) {
            //QuestProgress->HandleComplete();
        }
        //发放奖励
        auto& QuestRewards = FindQuest->GetQuestRewards();
        auto Character = UGameFrameworkUtils::GetCharacterFromComponentOwner(QuestComponent);
        for (auto QuestReward : QuestRewards) {
            QuestReward->HandleRewardDispatch(GameInstance, Character);
        }

        //这里表示任务完成
        QuestComponent->ExecutingQuests.RemoveAt(FindIndex);
        QuestComponent->FinishQuests.Add(FindQuest->GetQuest()->ID);
        QuestComponent->OnFinishQuestChanged();
        QuestComponent->OnQuestChanged();

        //取消监听
        GameInstance->GameSystemManager->GetSystemByClass<UConditionSystem>()->UnfollowConditions(FindQuest);
    }
    else {
        //todo，任务进度完成
        const auto& QuestProgresses = FindQuest->GetQuestProgresses();
        for (auto QuestProgress : QuestProgresses) {
            //QuestProgress->HandleComplete();
        }
        //发放奖励
        auto& QuestRewards = FindQuest->GetQuestRewards();
        auto Character = UGameFrameworkUtils::GetCharacterFromComponentOwner(QuestComponent);
        for (auto QuestReward : QuestRewards) {
            QuestReward->HandleRewardDispatch(GameInstance, Character);
        }
        
        //这里就跳转下个节点
        FindQuest->StepNextNode(StepIndex);
    }

    return true;
}

void UQuestSystem::AddExecutinQuestAfterLoaded(UQuestComponent* QuestComponent, const FGuid ID) {
    for (int Index = 0; Index < QuestComponent->ExecutingQuests.Num(); ++Index) {
        auto Quest = QuestComponent->ExecutingQuests[Index];
        if (Quest->GetID() == ID) {
            UE_LOG(GameCore, Warning, TEXT("任务已接受:%s"), *ID.ToString());

            return;
        }
    }
    UQuestForestSubsystem* QuestForestSubsystem = GameInstance->GetSubsystem<UQuestForestSubsystem>();
    auto Quest = QuestForestSubsystem->GetQuestByID(ID);
    if (!Quest) {
        UE_LOG(GameCore, Warning, TEXT("接受任务非法ID:%s"), *ID.ToString());

        return;
    }
    if (!Quest->QuestDetail.Get()) {
        UE_LOG(GameCore, Error, TEXT("非法任务，没有配置任务详情:%s"), *ID.ToString());
        return;
    }
    if (!Quest->QuestDetail->Root) {
        UE_LOG(GameCore, Error, TEXT("非法任务，任务详情根节点为空:%s"), *ID.ToString());
        return;
    }

    UExecutingQuest* NewExecuteQuest = NewObject<UExecutingQuest>(QuestComponent->GetOwner());
    NewExecuteQuest->Initialize(Quest);

    QuestComponent->ExecutingQuests.Add(NewExecuteQuest);
    QuestComponent->OnQuestChanged();

    UE_LOG(GameCore, Display, TEXT("接受任务成功, EID:%s, Uin:%s QuestID:%s"), *UGameFrameworkUtils::GetEntityID(QuestComponent->GetOwner()), *UGameFrameworkUtils::GetRoleID(QuestComponent->GetOwner()), *ID.ToString());
}

TArray<TSubclassOf<class UGameEventBase>> UQuestSystem::GetHandleEventTypes_Implementation() {
    return {
        UAcceptQuestRequesEvent::StaticClass(),
        UPushQuestRequesEvent::StaticClass(),
    };
}

void UQuestSystem::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
    if (HandleEvent->IsA(UAcceptQuestRequesEvent::StaticClass())) {
        auto Request = Cast<UAcceptQuestRequesEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto Character = Cast<APawn>(Entity.GetObject());
            if (Character) {
                auto PlayerState = Cast<ACoreCharacterStateBase>(Character->GetPlayerState());
                if (PlayerState && PlayerState->QuestComponent) {
                    AcceptQuest(PlayerState->QuestComponent, Request->ID);
                }
            }
        }
    }
    else if (HandleEvent->IsA(UPushQuestRequesEvent::StaticClass())) {
        auto Request = Cast<UPushQuestRequesEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto Character = Cast<APawn>(Entity.GetObject());
            if (Character) {
                auto PlayerState = Cast<ACoreCharacterStateBase>(Character->GetPlayerState());
                if (PlayerState && PlayerState->QuestComponent) {
                    FString Error;
                    PushQuest(PlayerState->QuestComponent, Request->ID, Request->StepIndex, Error);
                }
            }
        }
    }
}